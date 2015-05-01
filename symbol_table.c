#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tree.h"
#include "lemon.h"
#include "symbol_table.h"
#include "sym.h"

static int find_in_scope(symbol_table *table, int scope, char *want) {
        symbol_list *lst = &table->list[scope];
        //fprintf(stderr, "Finding symbol %s in scope %d:\n", want, scope);
        for (int i = 0; i < lst->count; i++) {
                //fprintf(stderr, "\tIs it \"%s\"? ", lst->symbols[i].symbol);
                if (strcmp(want, lst->symbols[i].symbol) == 0) {
                //        fprintf(stderr, "Yes!\n");
                        return i;
                } //else fprintf(stderr, "No.\n");
        }
        //fprintf(stderr, "Not found.\n");
        return -1;
}

sym_entry *find_symbol(symbol_table *tab, int smallest_scope, char *name) {
        int i = find_in_scope(tab, smallest_scope, name);
        if (i != -1) {
                return &tab->list[smallest_scope].symbols[i];
        }
        i = find_in_scope(tab, 0, name);
        if (i != -1) {
                return &tab->list[smallest_scope].symbols[i];
        }
        return NULL;
}

static int create_new_scope(symbol_table *tab) {
        tab->list = realloc(tab->list, sizeof(symbol_list)*(tab->len+1));
        tab->list[tab->len].symbols = NULL;
        tab->list[tab->len].count = 0;
        tab->len++;
        return tab->len-1;
}

static sym_entry *push_sym(symbol_list *list, sym_entry e) {
        list->symbols = realloc(list->symbols, (list->count+1)*sizeof(sym_entry));
        list->symbols[list->count] = e;
        list->count++;
        return &list->symbols[list->count-1];
}

static sym_entry *add_symbol(symbol_table *tab, int scope, tree_t *node, int type) {
        if (find_in_scope(tab, scope, node->attr.sval) != -1) {
                fprintf(stderr, "Error: in %dth function, %s is redeclared\n", scope, node->attr.sval);
                exit(-1);
        }
        sym_entry e;
        e.symbol = node->attr.sval;
        e.type = type;
        e.numargs = 0;//type != VAR ? count_args_prog(sym): 0;
        return push_sym(&tab->list[scope], e);
}

static void add_decls_to_scope(symbol_table *tab, int scope, tree_t *decls) {
        for (int i = 0; i < decls->nchildren; i++) {
                tree_t *decl = decls->children[i];
                int type = decl->children[1]->type;
                for (int j = 0; j < decl->children[0]->nchildren; j++) {
                        symbol_list *add_to = &tab->list[scope];
                        symbol_list t = *add_to;
                        add_symbol(tab, scope, decl->children[0]->children[j], type);
                }
        }
}

static int count_args(tree_t *subprogram) {
        tree_t *decls = subprogram->children[0]->children[1];
        int arg_count = 0;
        for (int i = 0; i < decls->nchildren; i++) {
                arg_count += decls->children[i]->children[0]->nchildren;
        }
        return arg_count;
}

// returns a block that sets the scope of a node to new_scope
static walkfunc make_set_scope_blk(int new_scope) {
        __block int i = new_scope;
        return Block_copy( ^(tree_t *root) {
                root->scope = i;
                return (char *)NULL;
        });
}

// adds the subprogram name to the top scope and all of its variables to an inner scope.
static void add_subprograms_to_scope(symbol_table *tab, int scope, tree_t *subprograms) {
        for (int i = 0; i < subprograms->nchildren; i++) {
                tree_t *subprog = subprograms->children[i];
                sym_entry *e = add_symbol(tab, scope,
                                                subprog->children[0]->children[0],
                                                subprog->children[0]->type);
                if (e->type == FUNCTION) {
                        int last = subprog->children[0]->nchildren - 1;
                        e->type = subprog->children[0]->children[last]->type;
                }
                e->numargs = count_args(subprog);
                int internal_scope = create_new_scope(tab);
                walkfunc set_scoper = make_set_scope_blk(internal_scope);
                walk_tree(subprog, set_scoper);
                add_decls_to_scope(tab, internal_scope, subprog->children[0]->children[1]);
                if (subprog->children[1]->type == DECLS) {
                        add_decls_to_scope(tab, internal_scope, subprog->children[1]);
                }
        }
}

symbol_table *create_symbol_table(tree_t *root, char **builtins, int num_builtins) {
        walk_tree(root, make_set_scope_blk(0));
        symbol_table *ret = malloc(sizeof(symbol_table));

        ret->list = malloc(sizeof(symbol_list));
        ret->len = 1;
        ret->list->symbols = NULL;
        ret->list->count = 0;

        for (int i = 0; i < num_builtins; i++) {
                tree_t temp;
                temp.type = PROCEDURE;
                temp.attr.sval = builtins[i];
                temp.nchildren = 0;
                temp.children = NULL;
                temp.scope = 0;
                add_symbol(ret, 0, &temp, PROCEDURE);
        }

        sym_entry *e;
        for (int i = 0; i < root->nchildren; i++) {
                switch (root->children[i]->type) {
                case ID: // top level id is program name
                        e = add_symbol(ret, 0, root->children[i], PROCEDURE);
                        // invariant: program is always declared as ex(foo, bar) with at least one
                        // thus it's first child has one child per arg.
                        // Not that I have any idea of what those can be used for...
                        e->numargs = root->children[i]->children[0]->nchildren;
                        for (int j = 0; j < root->children[i]->children[0]->nchildren; j++) {
                                add_symbol(ret, 0,
                                        root->children[i]->children[0]->children[j],
                                        VAR);
                        }
                        break;
                case DECLS:
                        add_decls_to_scope(ret, 0, root->children[i]);
                        break;
                case SUBPROGRAMS:
                        add_subprograms_to_scope(ret, 0, root->children[i]);
                        break;
                case STATEMENTS:
                        // better not have new variables in statements
                        break;
                default:
                        fprintf(stderr,
                                "Top level \"%s\" in AST unexpected?\n",
                                sym_to_string(root->children[i]->type));
                }

        }
        return ret;
}

static void print_symbol(sym_entry entry, int i) {
        fprintf(stderr,
                "symbol %d:\n\tid: %s\n\ttype: %s\n\targs: %d\n",
                i,
                entry.symbol,
                sym_to_string(entry.type),
                entry.numargs);
}

void print_symbol_table(symbol_table *tab) {
        fprintf(stderr, "scopes: %d\n", tab->len);
        for (int i = 0; i < tab->len; i++) {
                fprintf(stderr, "vars in scope %d: %d\n", i, tab->list[i].count);
                for (int j = 0; j < tab->list[i].count; j++) {
                        print_symbol(tab->list[i].symbols[j], j);
                }
        }
}

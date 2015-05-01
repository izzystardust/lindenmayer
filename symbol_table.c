#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tree.h"
#include "lemon.h"
#include "symbol_table.h"
#include "sym.h"

static int find_in_scope(symbol_table *table, int scope, char *want) {
        symbol_list lst = table->list[scope];

        for (int i = 0; i < lst.count; i++) {
                if (strcmp(want, lst.symbols[i].symbol) == 0) {
                        return i;
                }
        }
        return -1;
}

static sym_entry *push_sym(symbol_list *list, sym_entry e) {
        list->symbols = realloc(list->symbols, (list->count+1)*sizeof(sym_entry));
        list->symbols[list->count] = e;
        list->count++;
        return &list->symbols[list->count-1];
}

static sym_entry *add_symbol(symbol_list *to, tree_t *node, int type) {
        sym_entry e;
        e.symbol = node->attr.sval;
        e.type = type;
        e.numargs = 0;//type != VAR ? count_args_prog(sym): 0;
        return push_sym(to, e);
}

static void add_decls_to_scope(symbol_table *tab, int scope, tree_t *decls) {
        for (int i = 0; i < decls->nchildren; i++) {
                tree_t *decl = decls->children[i];
                int type = decl->children[1]->type;
                for (int j = 0; j < decl->children[0]->nchildren; j++) {
                        symbol_list *add_to = &tab->list[scope];
                        symbol_list t = *add_to;
                        add_symbol(add_to, decl->children[0]->children[j], type);
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

// adds the subprogram name to the top scope and all of its variables to an inner scope.
static void add_subprograms_to_scope(symbol_table *tab, int scope, tree_t *subprograms) {
        for (int i = 0; i < subprograms->nchildren; i++) {
                tree_t *subprog = subprograms->children[i];
                sym_entry *e = add_symbol(&tab->list[scope],
                                                subprog->children[0]->children[0],
                                                subprog->children[0]->type);
                e->numargs = count_args(subprog);
        }
}

// returns a block that sets the scope of a node to new_scope
static walkfunc make_set_scope_blk(int new_scope) {
        __block int i = new_scope;
        return Block_copy( ^(tree_t *root) {
                root->scope = i;
                return (char *)NULL;
        });
}

symbol_table *create_symbol_table(tree_t *root) {
        walk_tree(root, make_set_scope_blk(0));
        symbol_table *ret = malloc(sizeof(symbol_table));
        ret->list = malloc(sizeof(symbol_list));
        ret->len = 1;
        ret->list->symbols = NULL;
        ret->list->count = 0;
        sym_entry *e;
        for (int i = 0; i < root->nchildren; i++) {
                switch (root->children[i]->type) {
                case ID: // top level id is program name
                        e = add_symbol(ret->list, root->children[i], PROCEDURE);
                        // invariant: program is always declared as ex(foo, bar) with at least one
                        // thus it's first child has one child per arg.
                        // Not that I have any idea of what those can be used for...
                        e->numargs = root->children[i]->children[0]->nchildren;
                        break;
                case DECLS:
                        add_decls_to_scope(ret, 0, root->children[i]);
                        break;
                case SUBPROGRAMS:
                        add_subprograms_to_scope(ret, 0, root->children[i]);
                        break;
                case STATEMENTS:
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

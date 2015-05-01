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

static int count_args_prog(tree_t root) {
        // fprintf(stderr, "Counting args rooted at tree: ");
        // print_tree(&root);
        if (root.nchildren == 0) {
                return 0;
        }

        if (root.children[0]->type == ID) {
                return 1 + root.children[0]->nchildren;
        }

        return 0;
}

static int count_args_func(tree_t root) {
        if (root.nchildren < 1) {
                return 0;
        }

        if (root.children[0]->children[1]->type != DECL) {
                return 0;
        }

        return root.children[0]->children[1]->children[0]->nchildren + 1;
}

static void push_sym(symbol_list *list, sym_entry e) {
        list->symbols = realloc(list->symbols, (list->count+1)*sizeof(sym_entry));
        list->symbols[list->count] = e;
        list->count++;
}

static void add_symbol(symbol_list *list, tree_t sym, int type) {
        sym_entry e;
        e.symbol = sym.attr.sval;
        e.type = type;
        e.args = type != VAR ? count_args_prog(sym): 0;
        push_sym(list, e);
}

static sym_entry add_function_symbol(symbol_list *list, tree_t sym, int type) {
        // fprintf(stderr, "Adding function symbol rooted at\n");
        // print_tree(&sym);
        sym_entry e;
        e.symbol = sym.children[0]->children[0]->attr.sval;
        e.type = type;
        e.args = count_args_func(sym);
        push_sym(list, e);
        return e;
}

static int create_new_scope(symbol_table *tab) {
        tab->list = realloc(tab->list, sizeof(symbol_list)*(tab->len+1));
        tab->list[tab->len].symbols = NULL;
        tab->list[tab->len].count = 0;
        tab->len++;
        return tab->len-1;
}

walkfunc make_set_scope_blk(int new_scope) {
        __block int i = new_scope;
        return Block_copy( ^(tree_t *root) {
                root->scope = i;
                return (char *)NULL;
        });
}

static void handle_function_or_proc(symbol_table *tab, tree_t *decl) {
        sym_entry e = add_function_symbol(tab->list, *decl, PROCEDURE);
        if (e.args > 0) {
                int scope = create_new_scope(tab);
                walk_tree(decl, make_set_scope_blk(scope));
                tree_t *arg_base = decl->children[0]->children[1]->children[0];
                add_symbol(&tab->list[scope], *arg_base, VAR);
                for (int i = 0; i < arg_base->nchildren; i++) {
                        add_symbol(&tab->list[scope], *arg_base->children[i], VAR);
                }
        }
}

symbol_table *create_symbol_table(tree_t *root) {
        walk_tree(root, make_set_scope_blk(0));
        symbol_table *ret = malloc(sizeof(symbol_table));
        ret->list = malloc(sizeof(symbol_list));
        ret->list->symbols = NULL;
        ret->list->count = 0;
        ret->len = 1;
        for (int i = 0; i < root->nchildren; i++) {
                switch (root->children[i]->type) {
                case ID: // top level id is program name
                        add_symbol(ret->list, *(root->children[i]), PROCEDURE);
                        break;
                case VAR:
                        add_symbol(ret->list, *(root->children[i]->children[0]->children[0]), VAR);
                        for (int j = 0; j < root->children[i]->children[0]->children[0]->nchildren; j++) {
                                add_symbol(ret->list,
                                        *(root->children[i]->children[0]->children[0]->children[j]),
                                        VAR);
                        }
                        break;
                case PROGRAM:
                        // for (int j = 0; j < root->children[i]->nchildren; j++) {
                        //         fprintf(stderr, "Here\n");
                        //         handle_function_or_proc(ret, root->children[i]->children[j]);
                        // }
                        break;
                case PROCEDURE_CALL:
                        break;
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
                entry.args);
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

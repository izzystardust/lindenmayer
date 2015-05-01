#include <stdio.h>
#include <string.h>

#include "semantic.h"
#include "symbol_table.h"
#include "tree.h"
#include "lemon.h"

static char *check_prg_root(tree_t *root, symbol_table *table) {
        if (root->nchildren < 2) {
                return "Not enough stuff in your program";
        }
        return NULL;
}

static char *check_id_node(tree_t *root, symbol_table *table) {
        sym_entry *e = find_symbol(table, root->scope, root->attr.sval);
        if (!e) {
                char *fmt = "Symbol %s in function %d not declared.";
                char *buffer = malloc(sizeof(char) * strlen(fmt)-2 + strlen(root->attr.sval) + 10);
                sprintf(buffer, fmt, root->attr.sval, root->scope);
                ///fprintf(stderr, "In walk: %s\n", buffer);
                return buffer;
        }
        return NULL;
}

char *check_semantics(tree_t *root, symbol_table *table) {
        __block symbol_table *t = table;
        walkfunc checker = ^(tree_t *root) {
                if (!root) { return (char *)NULL; }
                if (root->type == PROGRAM && root->nchildren) {
                        return check_prg_root(root, table);
                }
                if (root->type == ID) {
                        return check_id_node(root, table);
                }
                return (char *)NULL;
        };

        return walk_tree(root, checker);
        //return (char *)NULL;
}

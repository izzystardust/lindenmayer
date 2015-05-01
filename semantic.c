#include "semantic.h"
#include "tree.h"
#include "lemon.h"

static char *check_prg_root(tree_t *root, symbol_table *table) {
        if (root->nchildren < 2) {
                return "Not enough stuff in your program";
        }
        return NULL;
}

char *check_semantics(tree_t *root, symbol_table *table) {
        __block symbol_table *t = table;
        walkfunc checker = ^(tree_t *root) {
                if (root && root->type == PROGRAM && root->nchildren) {
                        return check_prg_root(root, table);
                }
                return (char *)NULL;
        };
        return (char *)NULL;
}

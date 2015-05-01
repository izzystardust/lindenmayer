#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

#include "semantic.h"
#include "symbol_table.h"
#include "sym.h"
#include "tree.h"
#include "lemon.h"

// given a typed node, returns the type.
// given an untyped node, returns 0.
// given an invalidly typed node, returns 0 and sets errno.
int type_of(tree_t *root, symbol_table *tab) {
        assert(root != NULL);
        sym_entry *e;
        int t1, t2;
        switch (root->type) {
        case ID:
                e = find_symbol(tab, root->scope, root->attr.sval);
                if (e) {
                        return e->type;
                }
                errno = -1;
                return 0;
        case SUBPROGRAM:
                e = find_symbol(tab, root->scope, root->attr.sval);
                if (e && e->type != PROCEDURE) {
                        return e->type;
                }
                errno = -1;
                return 0;
        case MULOP: case ADDOP:
                t1 = type_of(root->children[0], tab);
                t2 = type_of(root->children[1], tab);
                return (t1==t2)? t1 : (errno = -1), 0;
        case RELOP: return BOOLEAN;
        case INUM:  return INTEGER;
        case RNUM:  return REAL;
        }

        fprintf(stderr, "Asked for Pascal type of node with AST type \"%s\" - invalid case?", sym_to_string(root->type));
        return 0;
}

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

static char *check_binop(tree_t *binop, symbol_table *tab) {
        int t1 = type_of(binop->children[0], tab);
        int t2 = type_of(binop->children[1], tab);
        if (t1 != t2) {
                char *buffer = malloc(sizeof(char)*256);
                sprintf(buffer, "Op with different types(t1: %s; t2: %s;)", sym_to_string(t1), sym_to_string(t2));
                return buffer;
        }
        return NULL;
}

static char *check_if(tree_t *iftr, symbol_table *tab) {
        if (type_of(iftr->children[0], tab) != BOOLEAN) {
                return "'if' condition must be boolean";
        }
        return NULL;
}

char *check_semantics(tree_t *root, symbol_table *table) {
        __block symbol_table *t = table;
        walkfunc checker = ^(tree_t *root) {
                if (!root) { return (char *)NULL; }
                switch (root->type) {
                case PROGRAM:
                        return check_prg_root(root, table);
                case ID:
                        return check_id_node(root, table);
                case MULOP: case ADDOP: case RELOP:
                        return check_binop(root, table);
                case IF:
                        return check_if(root, table);
                }
                return (char *)NULL;
        };

        return walk_tree(root, checker);
}

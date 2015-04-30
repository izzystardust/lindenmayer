#ifndef _tree_h_
#define _tree_h_

#include <stdlib.h>
#include <Block.h>

#include "union.h"

typedef struct tree_s {
	int type;
	attr_t attr;
	struct tree_s **children; // array of pointers to children
	size_t nchildren;
	int scope; // where to look up scope in
} tree_t;

tree_t *make_tree(int type, tree_t **children, size_t n);
tree_t *make_list(int type, tree_t *next);
tree_t *make_bint(lexer_item parent, tree_t *left, tree_t *right);
tree_t *make_leaf(lexer_item it);
tree_t *add_child(tree_t *parent, tree_t *child);

typedef char *(^walkfunc)(tree_t *node);

char *walk_tree(tree_t *root, walkfunc wf);

void print_tree(tree_t *t);

void easy_break();

#endif

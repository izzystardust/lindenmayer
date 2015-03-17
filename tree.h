#ifndef _tree_h_
#define _tree_h_

#include "union.h"

typedef struct tree_s {
	int type;
	attr_t attr;
	struct tree_s **children; // array of pointers to children
	size_t nchildren;
} tree_t;

tree_t *make_tree(int type, tree_t **children, size_t n);
tree_t *make_list(int type, tree_t *next);
tree_t *make_leaf(lexer_item it);
void add_child(tree_t *parent, tree_t *child);

void print_tree(tree_t *t);

void easy_break();

#endif

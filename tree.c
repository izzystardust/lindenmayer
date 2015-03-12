#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "tree.h"
#include "lemon.h"
#include "sym.h"

tree_t *make_tree(int type, tree_t **children, size_t n) {
	tree_t *p = malloc(sizeof(tree_t));
	assert(NULL != p);

	p->type = type;
	p->children = children;
	p->nchildren = n;

	return p;
}

tree_t *make_list(int type, tree_t *next) {
	return make_tree(type, &next, 1);
}

static void print_tree_helper(tree_t *t, int depth) {
	for (int i = 0; i < depth * 4; i++) {
		fprintf(stderr, " ");
	}
	fprintf(stderr, "%s\n", sym_to_string(t->type));
	for (int i = 0; i < t->nchildren; i++) {
		if (NULL != t->children[i]) {
			print_tree_helper(t->children[i], depth + 1);
		}
	}
}

void print_tree(tree_t *t) {
	print_tree_helper(t, 0);
}

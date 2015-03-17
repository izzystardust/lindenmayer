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
	tree_t **new = malloc(sizeof(tree_t **));
	new[0] = next;
	return make_tree(type, new, 1);
}

tree_t *make_leaf(lexer_item it) {
	tree_t *p = malloc(sizeof(tree_t));
	p->type = it.type;
	p->attr = it.attr;
	p->children = NULL;
	p->nchildren = 0;
	return p;
}

void add_child(tree_t *parent, tree_t *child) {
	// yeah, growing by one is slow
	fprintf(stderr, "children: %p", (void *)parent->children);
	//print_tree(parent);
	parent->children = realloc(parent->children, parent->nchildren+1);
	assert(NULL != parent->children);
	parent->children[parent->nchildren] = child;
	parent->nchildren++;
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

void easy_break() {}

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

tree_t *make_bint(lexer_item it, tree_t *left, tree_t *right) {
	tree_t **new = calloc(sizeof(tree_t *), 2);
	new[0] = left;
	new[1] = right;
	tree_t *ret =  make_tree(it.type, new, 2);
	ret->attr = it.attr;
	return ret;
}

tree_t *make_leaf(lexer_item it) {
	tree_t *p = malloc(sizeof(tree_t));
	p->type = it.type;
	p->attr = it.attr;
	p->children = NULL;
	p->nchildren = 0;
	return p;
}

tree_t *add_child(tree_t *parent, tree_t *child) {
	// yeah, growing by one is slow
	if (parent == NULL) {
		return make_list(child->type, child);
	}
	assert(NULL != parent);
	assert(NULL != child);
	parent->children = realloc(parent->children, sizeof(tree_t*)*(parent->nchildren+1));
	assert(NULL != parent->children);
	parent->children[parent->nchildren] = child;
	parent->nchildren++;
	return parent;
}

static void print_tree_helper(tree_t *t, int depth) {
	for (int i = 0; i < depth * 4; i++) {
		fprintf(stderr, " ");
	}
	fprintf(stderr, "%s", sym_to_string(t->type));
	if (ID == t->type || t->type < 0 || t->type > 100) {
		fprintf(stderr, ": %s (%d)", t->attr.sval, t->scope);
	}
	fprintf(stderr, "\n");
	for (int i = 0; i < t->nchildren; i++) {
		if (NULL != t->children[i]) {
			print_tree_helper(t->children[i], depth + 1);
		}
	}
}

#define I_AM_GROOT groot
// I am Groot.
char *walk_tree(tree_t *groot, walkfunc wf) {
	if (I_AM_GROOT) {
		char *err = wf(groot);
		if (err != NULL) { return err; }
		for (int i = 0; i < groot->nchildren; i++) {
			walk_tree(groot->children[i], wf);
		}
	}
	return NULL;
}

void print_tree(tree_t *t) {
	print_tree_helper(t, 0);
}

void easy_break() {}

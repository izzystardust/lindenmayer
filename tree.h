#ifndef _tree_h_
#define _tree_h_

typedef struct tree_s {
	int type;
	union {
		int ival;
		float rval;
		int opval;
		char *sval;
	} attr;
	struct tree_s **children; // array of pointers to children
	size_t nchildren;
} tree_t;

tree_t *make_tree(int type, tree_t **children, size_t n);
tree_t *make_list(int type, tree_t *next);

void print_tree(tree_t *t);

#endif

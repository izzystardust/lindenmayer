#include <stdlib.h>

// So, flex has an option %bison-bridge that gives the yylex function an
// additional argument that is the union structure from bison. I'm not using
// bison, so I need to do it's job with the YYSTYPE. I define the union in
// union.h, but scan.h refers to it and so it must be defined before scan.h
#define YYSTYPE attr_t

#include "lemon.h"
#include "union.h"
#include "scan.h"
#include "sglib.h"
#include "tree.h"
#include "semantic.h"
#include "gencode.h"
#include "symbol_table.h"

/* lemon functions */
void *ParseAlloc();
void Parse();
void ParseFree();

extern int parseerr; // set if a parse error occurs

int get_next_token(void *lexer, lexer_item *it) {
	it->type = yylex(&it->attr, lexer);
	return it->type;
}

tree_t *root = NULL;

tree_t * parse(FILE *f) {
	void *lexer;
	yylex_init(&lexer);
	void *parser = ParseAlloc(malloc);
	yyset_in(f, lexer);

	lexer_item val;

	while (get_next_token(lexer, &val)) {
		//lexer_item_print(val);
		Parse(parser, val.type, val, &root);
	}
	Parse(parser, 0, val, &root);
	ParseFree(parser, free);

	return root;
}

int main(int argc, char **argv) {
	errno = 0;
	FILE *in = stdin;
	if (argc > 1) {
		FILE *a = fopen(argv[1], "r");
		if (NULL != a) {
			in = a;
		}
	}
	tree_t * t = parse(in);
	if (parseerr) {
		fprintf(stderr, "Fix parse errors to get more errors\n");
		return -1;
	}

	//print_tree(t);
	char *builtins[] = {"read", "write"};
	symbol_table *table = create_symbol_table(root, builtins, 2);
	print_tree(t);
	//print_symbol_table(table);
	char *check = check_semantics(t, table);
	if (!check) {
		gencode(t, "a.out");
	} else {
		fprintf(stderr, "Did not pass semantic checking: %s\n", check);
	}
}

#include <stdlib.h>

#define YYSTYPE lexer_ut

#include "lemon.h"
#include "union.h"
#include "scan.h"
#include "sglib.h"

/* lemon functions */
void *ParseAlloc();
void Parse();
void ParseFree();

int get_next_token(void *lexer, lexer_item *it) {
	it->type = yylex(&it->attr, lexer);
	return it->type;
}

void parse(FILE *f) {
	void *lexer;
	yylex_init(&lexer);
	void *parser = ParseAlloc(malloc);
	yyset_in(f, lexer);

	lexer_item val;

	while (get_next_token(lexer, &val)) {
		lexer_item_print(val);
		Parse(parser, val.type, val);
	}
	Parse(parser, 0, val);
	ParseFree(parser, free);
}

int main(int argc, char **argv) {
	FILE *in = stdin;
	if (argc > 1) {
		FILE *a = fopen(argv[1], "r");
		if (NULL != a) {
			in = a;
		}
	}
	parse(in);
}

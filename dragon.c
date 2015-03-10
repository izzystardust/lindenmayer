#include <stdlib.h>

#include "lemon.h"
#include "union.h"
#include "scan.h"
#include "sglib.h"

/* lemon functions */
void *ParseAlloc();
void Parse();
void ParseFree();

int get_next_token(lexer_item *it) {
	it->type = yylex();
	it->attr.sval = strdup(yytext);
	return it->type;
}

void parse() {
	void *parser = ParseAlloc(malloc);
	lexer_item it;
	while (get_next_token(&it)) {
		//lexer_item_print(it);
		Parse(parser, it.type, it);
	}
	Parse(parser, 0, it);
	ParseFree(parser, free);
}

int main(int argc, char **argv) {
	if (argc > 1) {
		FILE *a = fopen(argv[1], "r");
		if (NULL != a) {
			yyin = a;
		}
	}
	parse();
}

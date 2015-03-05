#include <stdlib.h>

#include "lemon.h"
#include "scan.h"
#include "sglib.h"

/* lemon functions */
void *ParseAlloc();
void Parse();
void ParseFree();

int main() {
	void *lexer;
	yylex_init(&lexer);
	void *parser = ParseAlloc(malloc);
	for (;;) {
		int token = yylex();
		Parse(parser, token, yytext);
	}
}

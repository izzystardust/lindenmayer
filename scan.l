%{

#include <stdio.h>
#include <stdlib.h>
#include "lemon.h"

#define DEBUG_PRINT

// does this make me a bad person?
#ifdef DEBUG_PRINT
#define S_RET(it) 	fprintf(stderr, "{%s}", #it); return it
#define S_RETT(it) 	fprintf(stderr, "{%s[%s]}", #it, yytext); return it
#else
#define S_RET(it)	return it
#define S_RETT(it)	return it
#endif
%}

%option reentrant bison-bridge bison-locations noyywrap yylineno
%option full

ident		[a-zA-Z][a-zA-Z0-9]*
number		[0-9]+
whitespace	[ \t]+

%%

"program"	{ S_RET(PROGRAM); }
"array"		{ S_RET(ARRAY); }
"of"		{ S_RET(OF); }
"integer"	{ S_RET(INTEGER); }
"real"		{ S_RET(REAL); }
"function"	{ S_RET(FUNCTION); }
"proceduce"	{ S_RET(PROCEDURE); }
"begin"		{ S_RET(BBEGIN); }
"end"		{ S_RET(END); }
"if"		{ S_RET(IF); }
"then"		{ S_RET(THEN); }
"else"		{ S_RET(ELSE); }
"while"		{ S_RET(WHILE); }
"do"		{ S_RET(DO); }
"not"		{ S_RET(NOT); }
"var"		{ S_RET(VAR); }

".."		{ S_RET(DOTDOT); }
":="		{ S_RET(ASSIGNOP); }

"="		{ S_RETT(RELOP); }
"<>"		{ S_RETT(RELOP); }
"<"		{ S_RETT(RELOP); }
">"		{ S_RETT(RELOP); }
"<="		{ S_RETT(RELOP); }
">="		{ S_RETT(RELOP); }

"+"		{ S_RETT(ADDOP); }
"-"		{ S_RETT(ADDOP); }
"or"		{ S_RETT(ADDOP); }

"and"		{ S_RETT(MULOP); }
"*"		{ S_RETT(MULOP); }
"/"		{ S_RETT(MULOP); }
"div"		{ S_RETT(MULOP); }
"mod"		{ S_RETT(MULOP); }

{number}	{ S_RETT(NUM); }
{ident}		{ S_RETT(ID); }

{whitespace}	{ ; }
\n		{ fprintf(stderr, "%s", yytext); }
.		{ fprintf(stderr, "<%c>", yytext[0]); return yytext[0]; }

%%

void yyerror(char *msg)
{
	fprintf(stderr, "Error: %s\n", msg);
	exit(1);
}

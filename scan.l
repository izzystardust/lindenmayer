%{

#include <stdio.h>
#include <stdlib.h>
#include "lemon.h"
#include "union.h"

#define YYSTYPE attr_t

int linecount = 1;

%}

%option reentrant bison-bridge noyywrap yylineno
%option full

ident		[a-zA-Z][a-zA-Z0-9]*
number		[0-9]+
real            [0-9]+"."[0-9]*
whitespace	[ \t]+
comment         \(\*.*\*\)
oneline         [/][/](.*)[\n]

%%

"program"	{ return PROGRAM; }
"array"		{ return ARRAY; }
"of"		{ return OF; }
"integer"	{ return INTEGER; }
"real"		{ return REAL; }
"function"	{ return FUNCTION; }
"procedure"	{ return PROCEDURE; }
"begin"		{ return BBEGIN; }
"end"		{ return END; }
"if"		{ return IF; }
"then"		{ return THEN; }
"else"		{ return ELSE; }
"while"		{ return WHILE; }
"do"		{ return DO; }
"not"		{ return NOT; }
"var"		{ return VAR; }

".."		{ return DOTDOT; }
":="		{ return ASSIGNOP; }

"="		{ return RELOP; }
"<>"		{ return RELOP; }
"<"		{ return RELOP; }
">"		{ return RELOP; }
"<="		{ return RELOP; }
">="		{ return RELOP; }

"+"		{ return ADDOP; }
"-"		{ return ADDOP; }
"or"		{ return ADDOP; }

"and"		{ return MULOP; }
"*"		{ return MULOP; }
"/"		{ return MULOP; }
"div"		{ return MULOP; }
"mod"		{ return MULOP; }

"("             { return LPAREN; }
")"             { return RPAREN; }
"["             { return LBRKT; }
"]"             { return RBRKT; }
","		{ return COMMA; }
":"		{ return COLON; }
";"		{ return SEMI; }
"."		{ return DOT; }

{number}	{ yylval->ival = atoi(yytext); return INUM; }
{real}          { yylval->rval = atof(yytext); return RNUM; }
{ident}		{ yylval->sval = strdup(yytext); return ID; }

{whitespace}	{ ; }
{oneline}       { ; }
{comment}       { ; }
\n		{ fprintf(stderr, "%s", yytext); linecount++; }
.		{ fprintf(stderr, "\nYou forgot something: <%c>\n", yytext[0]); return 0; }

%%

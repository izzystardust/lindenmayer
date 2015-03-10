%{
#include <stdio.h>
#include <stdlib.h>
#include "parse.tab.h"
#include "tree.h"

int yylex();
void yyerror(const char *s);

%}

%union {
	int ival;
	float rval;
	char *sval;
	int opval;

	tree_t *tval;
}

%token NUM
%token ID

%token ASSIGNOP
%token RELOP
%token ADDOP
%token MULOP
%token NOT

%token PROGRAM
%token VAR
%token ARRAY OF DOTDOT
%token INTEGER REAL
%token FUNCTION PROCEDURE
%token BBEGIN END
%token IF THEN ELSE
%token WHILE DO

%%

program:
	PROGRAM ID '(' identifier_list ')' ';'
	declarations
	subprogram_declarations
	compound_statement
	'.'
	;

identifier_list
	: ID
	| identifier_list ',' ID
	;

declarations
	: declarations VAR identifier_list ':' type ';'
	| /* empty */
	;

type
	: standard_type
	| ARRAY '[' NUM DOTDOT NUM ']' OF standard_type
	;

standard_type
	: INTEGER
	| REAL
	;

subprogram_declarations
	: subprogram_declarations subprogram_declaration ';'
	| /* empty */
	;

subprogram_declaration
	: subprogram_head declarations compound_statement
	;

subprogram_head
	: FUNCTION ID arguments ':' standard_type ';'
	| PROCEDURE ID arguments ';'
	;

arguments
	: '(' parameter_list ')'
	| /* empty */
	;

parameter_list
	: identifier_list ':' type
	| parameter_list ';' identifier_list ':' type
	;

compound_statement:
	BBEGIN
	optional_statements
	END
	;

optional_statements
	: statement_list
	| /* empty */
	;

statement_list
	: statement
	| statement_list ';' statement
	;

statement
	: variable ASSIGNOP expression
	| procedure_statement
	| compound_statement
	| IF expression THEN statement ELSE statement
	| WHILE expression DO statement
	;

variable
	: ID
	| ID '(' expression_list ')'
	;

procedure_statement
	: ID
	| ID '(' expression_list ')'
	;

expression_list
	: expression
	| expression_list ',' expression
	;

expression
	: simple_expression
	| simple_expression RELOP simple_expression
	;

simple_expression
	: term
	| sign term
	| simple_expression ADDOP term
	;

term
	: factor
	| term MULOP factor
	;

factor
	: ID
	| ID '(' expression_list ')'
	| NUM
	| '(' expression ')'
	| NOT factor
	;

sign: '+' | '-';

%%

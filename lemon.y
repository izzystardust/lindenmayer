/*
 * This is a lemon parser
 * Because I'm a special snowflake
 */


%include {
	#include <stdlib.h>
	#include <assert.h>
	#include "lemon.h"
	#include "tree.h"

	extern tree_t *root;
}

%token_type     {tree_t *} // default type for terminals
%default_type   {tree_t *} // default type for nonterminals

// http://stackoverflow.com/questions/11705737/expected-token-using-lemon-parser-generator
%syntax_error {
	int n = sizeof(yyTokenName) / sizeof(yyTokenName[0]);
	for (int i = 0; i < n; ++i) {
		int a = yy_find_shift_action(yypParser, (YYCODETYPE)i);
		if (a < YYNSTATE + YYNRULE) {
			printf("possible token: %s\n", yyTokenName[i]);
		}
	}
}

%nonassoc INUM RNUM.
%nonassoc ID.
%nonassoc PLUS.
%nonassoc MINUS.
%nonassoc DOT.
%nonassoc SEMI COLON COMMA.
%nonassoc LPAREN RPAREN.
%nonassoc LBRKT RBRKT.

%nonassoc PROGRAM.
%nonassoc VAR.
%nonassoc ARRAY OF DOTDOT.
%nonassoc INTEGER REAL.
%nonassoc FUNCTION PROCEDURE.
%nonassoc BBEGIN END.
%nonassoc IF THEN ELSE.
%nonassoc WHILE DO.

%left ASSIGNOP.
%left RELOP.
%left ADDOP.
%left MULOP.
%left NOT.

program(A) ::=
	PROGRAM ID(I) LPAREN identifier_list(L) RPAREN SEMI
	declarations(D)
	subprogram_declarations(S)
	compound_statement(C)
	DOT.
	{
		int n = 5;
		tree_t **children = calloc(n, sizeof(tree_t *));
		children[0] = I;
		children[0] = NULL;
		children[1] = L;
		children[1] = NULL;
		children[2] = D;
		children[2] = NULL;
		children[3] = S;
		children[3] = NULL;
		children[4] = C;
		children[4] = NULL;
		root = make_tree(PROGRAM, children, n);
		fprintf(stderr, "t: %p\n", root);
		print_tree(root);
		A = root;
	}
identifier_list ::= ID.
identifier_list ::= identifier_list COMMA ID.

declarations ::= declarations VAR identifier_list COLON type SEMI.
declarations ::= .

type ::= standard_type.
type ::= ARRAY LBRKT NUM DOTDOT NUM RBRKT OF standard_type.

standard_type ::= INTEGER.
standard_type ::= REAL.

subprogram_declarations ::= subprogram_declarations subprogram_declaration SEMI.
subprogram_declarations ::= .

subprogram_declaration ::= subprogram_head declarations compound_statement.

subprogram_head ::= FUNCTION ID arguments COLON standard_type SEMI.
subprogram_head ::= PROCEDURE ID arguments SEMI.

arguments ::= LPAREN parameter_list RPAREN.
arguments ::= .

parameter_list ::= identifier_list COLON type.
parameter_list ::= parameter_list SEMI identifier_list COLON type.

compound_statement ::= BBEGIN optional_statements END.

optional_statements ::= statement_list.
optional_statements ::= .

statement_list ::= statement.
statement_list ::= statement_list SEMI statement.

statement ::= variable ASSIGNOP expression.
statement ::= procedure_statement.
statement ::= compound_statement.
statement ::= IF expression THEN statement ELSE statement.
statement ::= WHILE expression DO statement.

variable ::= ID.
variable ::= ID LPAREN expression_list RPAREN.

procedure_statement ::= ID.
procedure_statement ::= ID LPAREN expression_list RPAREN.

expression_list ::= expression.
expression_list ::= expression_list COMMA expression.

expression ::= simple_expression.
expression ::= simple_expression RELOP simple_expression.

simple_expression ::= term.
simple_expression ::= sign term.
simple_expression ::= simple_expression ADDOP term.

term ::= factor.
term ::= term MULOP factor.

factor(A) ::= ID(B). { A = make_list(ID, B); }
factor ::= ID LPAREN expression_list RPAREN.
factor(A) ::= INUM(B). { A = make_list(INUM, B); }
factor(A) ::= RNUM(B). { A = make_list(RNUM, B); }
factor(A) ::= LPAREN expression(B) RPAREN. { A = B; }
factor(A) ::= NOT factor(B). { A = make_list(NOT, B); }

sign ::= PLUS.
sign ::= MINUS.

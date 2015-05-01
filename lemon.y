/*
 * This is a lemon parser
 * Because I'm a special snowflake
 */


%include {
        #include <stdio.h>
	#include <stdlib.h>
	#include <assert.h>
	#include "union.h"
	#include "lemon.h"
	#include "tree.h"
        #include "sym.h"

	extern tree_t *root;
}

%token_type     {lexer_item}            // default type for terminals
%default_type   {tree_t *}            // default type for nonterminals
%extra_argument {tree_t **root} // lexer context

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

%nonassoc PROGRAM SUBPROGRAMS SUBPROGRAM.
%nonassoc VAR.
%nonassoc ARRAY OF DOTDOT.
%nonassoc INTEGER REAL.
%nonassoc FUNCTION PROCEDURE.
%nonassoc BBEGIN END.
%nonassoc IF THEN ELSE.
%nonassoc WHILE DO.

%nonassoc STATEMENTS.
%nonassoc PROCEDURE_CALL.
%nonassoc DECL DECLS.
%nonassoc IDONTKNOWYET.

%left ASSIGNOP.
%left RELOP.
%left ADDOP.
%left MULOP.
%left NOT.

program(P) ::=
	PROGRAM ID(I) LPAREN identifier_list(L) RPAREN SEMI
	declarations(D)
	subprogram_declarations(S)
	compound_statement(C)
	DOT.
	{
		//easy_break();
		int n = 4;
		tree_t **children = calloc(n, sizeof(tree_t *));
		//children[0] = I;
		//lexer_item_print(*I);
		children[0] = add_child(make_leaf(I), L);
		children[1] = D;
		children[2] = S;
		children[3] = C;
		P = make_tree(PROGRAM, children, n);
                *root = P;

                // walkfunc elide_double = ^(tree_t *node) {
                //         // this looks for children of node that have exactly one
                // };
	}
identifier_list(L) ::= ID(I). { L = make_list(VAR, make_leaf(I)); }
identifier_list(L) ::= identifier_list(B) COMMA ID(I). {
        L = add_child(B, make_leaf(I));
}

declarations(D) ::= declarations(S) VAR identifier_list(L) COLON type(T) SEMI. {
        tree_t ** c = calloc(sizeof(tree_t *), 2);
        int type = DECL;
        c[0] = L;
        c[1] = T;
        D = add_child(S, make_tree(type, c, 2));
}
declarations(D) ::= . {
        D = make_list(DECLS, NULL);
}

type(T) ::= standard_type(S). { T = S; }
type(T) ::= ARRAY LBRKT NUM(N) DOTDOT NUM(M) RBRKT OF standard_type(S). {
        tree_t ** c = calloc(sizeof(tree_t *), 3);
        c[0] = make_leaf(N);
        c[1] = make_leaf(M);
        c[2] = S;
        T = make_tree(ARRAY, c, 3);
}

standard_type(T) ::= INTEGER(I). { T = make_leaf(I); }
standard_type(T) ::= REAL(R).    { T = make_leaf(R); }

subprogram_declarations(D) ::=
        subprogram_declarations(S)
        subprogram_declaration(N) SEMI.
        {
                D = add_child(S, N);
        }
subprogram_declarations(D) ::= . {
        D = make_list(SUBPROGRAMS, NULL);
}

subprogram_declaration(D) ::=
        subprogram_head(H)
        declarations(E)
        compound_statement(C).
        {
                tree_t ** ch = calloc(sizeof(tree_t *), 3);
                ch[0] = H;
                ch[1] = E;
                ch[2] = C;
                D = make_tree(SUBPROGRAM, ch, 3);
        }

subprogram_head(H) ::= FUNCTION ID(I) arguments(A) COLON standard_type(T) SEMI.{
        tree_t ** ch = calloc(sizeof(tree_t *), 3);
        ch[0] = make_leaf(I);
        ch[1] = A;
        ch[2] = T;
        H = make_tree(FUNCTION, ch, 3);
}
subprogram_head(H) ::= PROCEDURE ID(I) arguments(A) SEMI. {
        tree_t ** ch = calloc(sizeof(tree_t *), 2);
        ch[0] = make_leaf(I);
        ch[1] = A;
        H = make_tree(PROCEDURE, ch, 2);
}

arguments(A) ::= LPAREN parameter_list(L) RPAREN. { A = L; }
arguments(A) ::= . { A = make_list(DECLS, NULL); }

parameter_list(P) ::= identifier_list(I) COLON type(T). {
        tree_t ** ch = calloc(sizeof(tree_t *), 2);
        ch[0] = I;
        ch[1] = T;
        P = make_list(DECLS, make_tree(DECL, ch, 2));
}
parameter_list(P) ::= parameter_list(L) SEMI identifier_list(I) COLON type(T). {
        tree_t ** ch = calloc(sizeof(tree_t *), 2);
        ch[0] = I;
        ch[1] = T;
        tree_t *t = make_tree(DECL, ch, 2);
        P = add_child(L, t);
}

compound_statement(C) ::= BBEGIN optional_statements(O) END. { C = O; }

optional_statements(O) ::= statement_list(L). { O = L; }
optional_statements(O) ::= . { O = make_list(STATEMENTS, NULL); }

statement_list(L) ::= statement(S). { L = make_list(STATEMENTS, S); }
statement_list(L) ::= statement_list(I) SEMI statement(T). {
        L = add_child(I, T);
}

statement(S) ::= variable(V) ASSIGNOP(A) expression(E). {
        tree_t **children = calloc(sizeof(tree_t *), 3);
        children[0] = make_leaf(A);
        children[1] = V;
        children[2] = E;
        S = make_tree(ASSIGNOP, children, 3);
}
statement(S) ::= procedure_statement(P). { S = P; }
statement(S) ::= compound_statement(C). { S = C; }
statement(S) ::= IF expression(E) THEN statement(T) ELSE statement(U). {
        tree_t **children = calloc(sizeof(tree_t *), 3);
        children[0] = E;
        children[1] = T;
        children[2] = U;
        S = make_tree(IF, children, 3);
}

statement(S) ::= WHILE expression(E) DO statement(T). {
        tree_t **children = calloc(sizeof(tree_t *), 2);
        children[0] = E;
        children[1] = T;
        S = make_tree(WHILE, children, 2);
}

variable(V) ::= ID(I). { V = make_leaf(I); }
variable(V) ::= ID(I) LPAREN expression_list(E) RPAREN. {
        tree_t ** children = calloc(sizeof(tree_t *), 2);
        children[0] = make_leaf(I);
        children[1] = E;
        V = make_tree(FUNCTION, children, 2);
}

procedure_statement(P) ::= ID(I). {
        P = make_list(PROCEDURE_CALL, make_leaf(I));
}
procedure_statement(P) ::= ID(I) LPAREN expression_list(L) RPAREN. {
        tree_t **list = calloc(sizeof(tree_t *), 2);
        list[0] = make_leaf(I);
        list[1] = L;
        P = make_tree(PROCEDURE_CALL, list, 2);
}

expression_list(L) ::= expression(E). { L = E; }
expression_list(L) ::= expression_list(R) COMMA expression(E). {
        L = add_child(R, E);
}

expression(E) ::= simple_expression(S). { E = S; }
expression(E) ::= simple_expression(L) RELOP(O) simple_expression(R). {
        E = make_bint(O, L, R);
}

simple_expression(E) ::= term(T). { E = T; }
simple_expression(E) ::= sign term(T). { E = T; }
simple_expression(E) ::= simple_expression(S) ADDOP(A) term(T). {
        E = make_bint(A, S, T);
}

term(T) ::= factor(F). { T = F; }
term(T) ::= term(L) MULOP(O) factor(R). {
	T = make_bint(O, L, R);
}

factor(A) ::= ID(B). { A = make_leaf(B); }
factor(A) ::= ID(I) LPAREN expression_list(L) RPAREN. {
	// function or procedure call
	tree_t ** children = calloc(sizeof(tree_t*), 2);
	children[0] = make_leaf(I);
	children[1] = L;

	A = make_tree(FUNCTION, children, 2);
}
factor(A) ::= INUM(I). { A = make_leaf(I); }
factor(A) ::= RNUM(R). { A = make_leaf(R); }
factor(A) ::= LPAREN expression(B) RPAREN. { A = B; }
factor(A) ::= NOT factor(B). { A = make_list(NOT, B); }

sign ::= PLUS.
sign ::= MINUS.

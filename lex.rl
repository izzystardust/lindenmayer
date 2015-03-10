#include <string.h>
#include <stdlib.h>
#include <lemon.h>

// does this make me a bad person?
#ifdef DEBUG_PRINT
#define S_RET(it) 	fprintf(stderr, "{%s}", #it); return it
#define S_RETT(it) 	fprintf(stderr, "{%s[%s]}", #it, yytext); return it
#else
#define S_RET(it)	return it
#define S_RETT(it)	return it
#endif

void token(int tok) {
	char *data = ts;
	int len = te-ts;
	fprintf(stderr, "<%d>", tok);
}

%%{

	machine pascal_lexer;

	digits = digit digit*;
	optional_fraction = '.' digits | empty;
	optional_exponent = ( 'E' ('+' | '-' | empty) digits | empty;
	num = digits optional_fraction optional_exponent;

	main := |*
		'program' =>    { S_RET(PROGRAM); };
		'array' =>      { S_RET(ARRAY); };
		'of' =>         { S_RET(OF); };
		'integer' =>    { S_RET(INTEGER); };
		'real' =>       { S_RET(REAL); };
		'function' =>   { S_RET(FUNCTION); };
		'procedure' =>  { S_RET(PROCEDURE); };
		'begin' =>      { S_RET(BEGIN); };
		'end' =>        { S_RET(END); };
		'if' =>         { S_RET(IF); };
		'then' =>       { S_RET(THEN); };
		'else' =>       { S_RET(ELSE); };
		'while' =>      { S_RET(WHILE); };
		'do' =>         { S_RET(DO); };
		'not' =>        { S_RET(NOT); };
		'var' =>        { S_RET(VAR); };

		'..' =>         { S_RET(DOTDOT); };
		':=' =>         { S_RET(ASSIGNOP); };

		'(' =>          { S_RET(LPAREN); };
		')' =>          { S_RET(RPAREN); };
		'[' =>          { S_RET(RBRKT); };
		']' =>          { S_RET(LBRKT); };

		'=' =>          { S_RETT(RELOP); };
		'<>' =>         { S_RETT(RELOP); };
		'<' =>          { S_RETT(RELOP); };
		'>' =>          { S_RETT(RELOP); };
		'<=' =>         { S_RETT(RELOP); };
		'>=' =>         { S_RETT(RELOP); };

		'+' =>          { S_RETT(ADDOP); };
		'-' =>          { S_RETT(ADDOP); };
		'or' =>         { S_RETT(ADDOP); };

		'and' =>        { S_RETT(MULOP); };
		'*' =>          { S_RETT(MULOP); };
		'/' =>          { S_RETT(MULOP); };
		'div' =>        { S_RETT(MULOP); };
		'mod' =>        { S_RETT(MULOP); };
	*|;

}%%

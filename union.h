#ifndef _union_h_
#define _union_h_

typedef union lexer_u {
	int ival;
	float rval;
	char *sval;
} lexer_ut;

typedef struct lexer_item_s {
	int type;
	lexer_ut attr;
} lexer_item;

void lexer_item_print(lexer_item it);
#endif

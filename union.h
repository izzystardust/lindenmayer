#ifndef _union_h_
#define _union_h_

typedef union attr_u {
	int ival;
	int opval;
	float rval;
	char *sval;
} attr_t;

typedef struct lexer_item_s {
	int type;
	attr_t attr;
} lexer_item;

void lexer_item_print(lexer_item it);
#endif

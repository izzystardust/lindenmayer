#ifndef _union_h_
#define _union_h_

#include <stdio.h>

typedef struct lexer_item_s {
	int type;
	union {
		int ival;
		float rval;
		char *sval;
	} attr;
} lexer_item;

void lexer_item_print(lexer_item it) {
	//TODO: switch on type
	fprintf(stderr, "[%d(%s)]\n", it.type, it.attr.sval);
}

#endif

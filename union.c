#include <stdio.h>

#include "union.h"
#include "sym.h"
#include "lemon.h"

void lexer_item_print(lexer_item it) {
	switch (it.type) {
		case INUM: fprintf(stderr, "[INUM:%d]", it.attr.ival); break;
		case RNUM: fprintf(stderr, "[RNUM:%f]", it.attr.rval); break;
		case ID: fprintf(stderr, "[ID:%s]", it.attr.sval); break;
		default:
			   fprintf(stderr, "[%s]", sym_to_string(it.type)); break;
	}
}


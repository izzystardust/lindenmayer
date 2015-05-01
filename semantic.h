#ifndef _semantic_h_
#define _semantic_h_

#include "tree.h"
#include "symbol_table.h"

// returns nil if passes or an error string if not
char *check_semantics(tree_t *root, symbol_table *table);

int type_of(tree_t *root, symbol_table *table);

#endif

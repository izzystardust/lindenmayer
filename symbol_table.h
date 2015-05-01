#ifndef symbol_table_h
#define symbol_table_h

typedef struct sym_entry_s {
        char *symbol;
        int type;
        int numargs;
        int args[]; // array containing numargs elements, each one the type of the argument
} sym_entry;

typedef struct symbol_list_s {
        sym_entry *symbols;
        int count;
} symbol_list;

typedef struct symbol_table_s {
        symbol_list *list;
        int len;
} symbol_table;

symbol_table *create_symbol_table(tree_t *root);
void print_symbol_table(symbol_table *tab);
sym_entry *find_symbol(symbol_table *tab, int smallest_scope, char *name);

#endif

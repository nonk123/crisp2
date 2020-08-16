#ifndef EVAL_H
#define EVAL_H

#include "parse.h"

struct SymbolTableNode {
    char* key;
    struct Value* value;
    struct SymbolTableNode* next;
};

struct SymbolTableNode* new_symbol_table_node(char*, struct Value*);

void put_symbol(char*, struct Value*);

struct Value* lookup_symbol(char*);

struct Value* eval(struct Value*);

#endif

#include "eval.h"

#include <stdlib.h>

static struct SymbolTableNode* symbol_table = NULL;

struct SymbolTableNode* new_symbol_table_node(char* key, struct Value* value) {
    struct SymbolTableNode* node = malloc(sizeof(struct SymbolTableNode));
    node->key = key;
    node->value = value;
    node->next = NULL;
    return node;
}

void put_symbol(char* key, struct Value* value) {
    make_static(value);

    if (!symbol_table) {
        symbol_table = new_symbol_table_node(key, value);
        return;
    }

    struct SymbolTableNode* node = symbol_table;

    for (;;) {
        if (!node->next) break;
        node = node->next;
    }

    node->next = new_symbol_table_node(key, value);
}

struct Value* lookup_symbol(char* key) {
    struct SymbolTableNode* node = symbol_table;

    while (node) {
        if (!strcmp(node->key, key))
            return node->value;
        node = node->next;
    }

    return NULL;
}

struct Value* eval_symbol(struct Value* value) {
    struct Value* found = lookup_symbol(value->symbol);

    free_value(value);

    if (found)
        return found;

    return new_error("symbol is void");
}

struct Value* eval_list(struct Value* value) {
    struct Value* result = new_value();
    struct ListNode* node = value->list;

    struct ListNode* start = new_list_node();
    struct ListNode* list = start;

    while (node && node->value) {
        struct Value* evaluated = eval(node->value);

        if (evaluated->error) {
            free_value(result);
            free_value(value);
            return evaluated;
        }

        set_ownership(evaluated, result);

        list->value = evaluated;
        list->next = new_list_node();
        list = list->next;

        node = node->next;
    }

    result->list = start;

    free_value(value);

    return result;
}

struct Value* eval(struct Value* value) {
    if (value->quoted)
        return value;

    if (value->symbol)
        return eval_symbol(value);
    else if (value->list)
        return eval_list(value);
    else
        return value;
}

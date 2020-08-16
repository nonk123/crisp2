#ifndef PARSER_H
#define PARSER_H

#include <stddef.h>
#include <string.h>
#include <stdbool.h>

#define PARSER(name) struct Value* name(struct Context* ctx)

#define OVER_WORD while (*ctx->at > ' ' && *ctx->at != ')')
#define PASS return NULL

struct Value;

struct ListNode {
    struct Value* value;
    struct ListNode* next;
};

struct Value {
    int* integer;
    char* symbol;
    bool quoted;
    struct ListNode* list;
    struct Value* lifetime;
    const char* error;
};

struct ListNode* new_list_node();

void free_list_node(struct ListNode*, struct Value*);

struct Value* new_value();

struct Value* new_error(const char*);

void make_static(struct Value*);

void set_ownership(struct Value*, struct Value*);

void set_quoted(struct Value*, bool);

void free_owned_value(struct Value*, struct Value*);

void free_value(struct Value*);

struct Context {
    size_t size;
    char* buffer;
    size_t position;
    char* at;
};

struct Context* new_ctx();

void next(struct Context*);

void prev(struct Context*);

size_t buffer_len(struct Context*);

void free_ctx(struct Context*);

void print_value(struct Context*, struct Value*, int);

PARSER(parse_list);
PARSER(parse_symbol);
PARSER(parse_integer);
PARSER(parse_token);
PARSER(parse);

struct Value* quick_parse(const char*);

#endif

#ifndef PARSER_H
#define PARSER_H

#include <stddef.h>
#include <string.h>

#define PARSER(name) struct Value* name(struct Context* ctx)

#define OVER_WORD while (*ctx->at > ' ' && *ctx->at != ')')
#define PASS return NULL

enum Lifetime {
    STATIC,
    SCOPED,
    OWNED,
};

struct Value {
    int* integer;
    char* symbol;
    int quoted;
    struct Value* list;
    struct Value* next;
    enum Lifetime lifetime;
    const char* error;
};

struct Value* new_value();

struct Value* new_error(const char*);

void set_quoted(struct Value*, int);

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

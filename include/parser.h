#ifndef PARSER_H
#define PARSER_H

#include <stddef.h>
#include <string.h>

#define PARSER(name) struct ParserResult* name(struct ParserContext* ctx)

#define OVER_WORD while (*ctx->at > ' ' && *ctx->at != ')')
#define PASS return NULL

struct ParserResult {
    int* integer;
    char* symbol;
    int quoted;
    struct ParserResult* next;
    struct ParserResult* list;
    const char* error;
};

struct ParserResult* new_result();

struct ParserResult* new_error(const char*);

void set_quoted(struct ParserResult*, int);

void free_result(struct ParserResult*);

struct ParserContext {
    size_t size;
    char* buffer;
    size_t position;
    char* at;
};

struct ParserContext* new_ctx();

void next(struct ParserContext*);

void prev(struct ParserContext*);

size_t buffer_len(struct ParserContext*);

void free_ctx(struct ParserContext*);

void print_result(struct ParserContext*, struct ParserResult*, int);

PARSER(parse_list);
PARSER(parse_symbol);
PARSER(parse_integer);
PARSER(parse_token);
PARSER(parse);

#endif

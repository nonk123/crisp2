#ifndef PARSER_H
#define PARSER_H

#include <stddef.h>
#include <string.h>

#define PARSER(name) ParserResult* name(ParserContext* ctx)

#define ITERATE_WORD while(*ctx->buffer > 32)
#define NEXT do { ctx->position++; ctx->buffer++; } while (0)
#define CHAR *ctx->buffer
#define AHEAD ctx->buffer[1]

#define RESULT(field, value)                                    \
    do {                                                        \
        ParserResult* result = malloc(sizeof(ParserResult));    \
        result->field = value;                                  \
        return result;                                          \
    } while (0)
#define ERROR(message) RESULT(error, message)
#define PASS return NULL

#define CALL_PARSER(name)                       \
    do {                                        \
        ParserResult* result = name(ctx);       \
        if (result) return result;              \
    } while (0)

typedef struct {
    int integer;
    char* symbol;
    const char* error;
} ParserResult;

void free_result(ParserResult*);

typedef struct {
    size_t position;
    size_t size;
    char* buffer;
} ParserContext;

ParserContext* new_ctx();

size_t buffer_len(ParserContext*);

void free_ctx(ParserContext*);

PARSER(parse_symbol);
PARSER(parse_integer);
PARSER(parse);

#endif

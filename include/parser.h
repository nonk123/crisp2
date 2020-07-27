#ifndef PARSER_H
#define PARSER_H

#include <stddef.h>
#include <string.h>

#define PARSER(name) ParserResult* name(ParserContext* ctx)

#define ITERATE_WORD while(*ctx->buffer > 32)
#define NEXT do { ctx->position++; ctx->buffer++; } while (0)

#define ALLOC(variable, type, value)            \
    type * variable = malloc(sizeof(type));     \
    *variable = value
#define RESULT(field, value)                                    \
    do {                                                        \
        ParserResult* result = malloc(sizeof(ParserResult));    \
        result->field = value;                                  \
        return result;                                          \
    } while (0)
#define ERROR(message) RESULT(error, message)

typedef struct {
        int integer;
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

int is_digit(char c);

PARSER(parse_integer);
PARSER(parse);

#endif

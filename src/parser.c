#include <stdlib.h>
#include <stdio.h>

#include "parser.h"

struct ParserResult* new_result() {
    return malloc(sizeof(struct ParserResult));
}

struct ParserResult* new_error(const char* message) {
    struct ParserResult* result = new_result();
    result->error = message;
    return result;
}

void free_result(struct ParserResult* result) {
    if (result->symbol)
        free(result->symbol);

    free(result);
}

struct ParserContext* new_ctx() {
    struct ParserContext* ctx = malloc(sizeof(struct ParserContext));

    ctx->size = 1024;
    ctx->buffer = calloc(ctx->size, sizeof(char));
    ctx->at = ctx->buffer;
    ctx->position = 0;

    return ctx;
}

void next(struct ParserContext* ctx) {
    ctx->at++;
    ctx->position++;
}

size_t buffer_len(struct ParserContext* ctx) {
    return strlen(ctx->buffer);
}

void free_ctx(struct ParserContext* ctx) {
    if (ctx->buffer)
        free(ctx->buffer);

    free(ctx);
}

int is_digit(char c) {
    return c >= '0' && c <= '9';
}

int is_valid_identifier(char c) {
    if (c >= 'a' && c <= 'z')
        return 1;

    if (c >= 'A' && c <= 'Z')
        return 1;

    if (c >= '0' && c <= '9')
        return 1;

    if (c >= '#' && c <= '&')
        return 1;

    if (c == '*' || c == '+')
        return 1;

    if (c >= '-' && c <= '/')
        return 1;

    if (c >= '<' && c <= '@')
        return 1;

    if (c == '!' || c == ':' || c == '~' || c == '^' || c == '_' || c == '|')
        return 1;

    return 0;
}

PARSER(parse_symbol) {
    struct ParserResult* result = new_result();
    result->symbol = calloc(ctx->size, sizeof(char));

    size_t index = 0;

    OVER_WORD {
        if (!is_valid_identifier(*ctx->at)) {
            free_result(result);
            return new_error("not a valid identifier");
        }

        result->symbol[index++] = *ctx->at;

        next(ctx);
    }

    return result;
}

PARSER(parse_integer) {
    int sign = 0;

    if (*ctx->at == '-') sign = -1;
    else sign = 1;

    if (!is_digit(*ctx->at)) {
        if (!is_digit(ctx->at[1]))
            PASS;
        next(ctx);
    }

    struct ParserResult* result = new_result();
    result->integer = 0;

    OVER_WORD {
        if (!is_digit(*ctx->at)) {
            free_result(result);
            return new_error("expected a digit");
        }

        int digit = *ctx->at - '0';
        result->integer = result->integer * 10 + digit;

        next(ctx);
    }

    result->integer *= sign;

    return result;
}

#define CALL_PARSER(parser)                             \
    do {                                                \
        struct ParserResult* result = parser(ctx);      \
        if (result) return result;                      \
    } while (0)

PARSER(parse) {
    while (*ctx->at <= ' ' && *ctx->at != '\0')
        next(ctx);

    if (buffer_len(ctx) == 0 || *ctx->at == '\0')
        return new_error("empty buffer");

    if (*ctx->at == '-' || *ctx->at == '+')
        CALL_PARSER(parse_integer);

    if (is_digit(*ctx->at))
        CALL_PARSER(parse_integer);

    if (is_valid_identifier(*ctx->at))
        CALL_PARSER(parse_symbol);

    return new_error("no parser found for input");
}

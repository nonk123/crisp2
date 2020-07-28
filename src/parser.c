#include <stdlib.h>
#include <stdio.h>

#include "parser.h"

void free_result(ParserResult* result) {
    if (result->symbol)
        free(result->symbol);
    free(result);
}

ParserContext* new_ctx() {
    ParserContext* ctx = malloc(sizeof(ParserContext));

    ctx->size = 1024;
    ctx->position = 0;
    ctx->buffer = calloc(ctx->size, sizeof(char));

    return ctx;
}

size_t buffer_len(ParserContext* ctx) {
    return strlen(ctx->buffer);
}

void free_ctx(ParserContext* ctx) {
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
    char* symbol = calloc(ctx->size, sizeof(char));

    ITERATE_WORD {
        if (!is_valid_identifier(CHAR)) {
            free(symbol);
            ERROR("not a valid identifier");
        }

        symbol[ctx->position] = CHAR;

        NEXT;
    }

    RESULT(symbol, symbol);
}

PARSER(parse_integer) {
    int sign = 0;

    if (CHAR == '-') sign = -1;
    else sign = 1;

    if (!is_digit(CHAR)) {
        if (!is_digit(AHEAD))
            PASS;
        NEXT;
    }

    int number = 0;

    ITERATE_WORD {
        if (!is_digit(CHAR))
            ERROR("expected a digit");

        int digit = CHAR - '0';
        number = number * 10 + digit;

        NEXT;
    }

    number *= sign;

    RESULT(integer, number);
}

PARSER(parse) {
    if (buffer_len(ctx) == 0 || CHAR <= 32)
        ERROR("empty buffer");

    if (CHAR == '-' || CHAR == '+')
        CALL_PARSER(parse_integer);

    if (is_digit(CHAR))
        CALL_PARSER(parse_integer);

    if (is_valid_identifier(CHAR))
        CALL_PARSER(parse_symbol);

    ERROR("no parser found for input");
}

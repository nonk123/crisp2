#include <stdlib.h>
#include <stdio.h>

#include "parser.h"

ParserResult* new_result() {
    return malloc(sizeof(ParserResult));
}

ParserResult* new_error(const char* message) {
    ParserResult* result = new_result();
    result->error = message;
    return result;
}

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
    ParserResult* result = new_result();
    result->symbol = calloc(ctx->size, sizeof(char));

    size_t index = 0;

    ITERATE_WORD {
        if (!is_valid_identifier(CHAR)) {
            free_result(result);
            return new_error("not a valid identifier");
        }

        result->symbol[index++] = CHAR;

        NEXT;
    }

    return result;
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

    ParserResult* result = new_result();
    result->integer = 0;

    ITERATE_WORD {
        if (!is_digit(CHAR)) {
            free_result(result);
            return new_error("expected a digit");
        }

        int digit = CHAR - '0';
        result->integer = result->integer * 10 + digit;

        NEXT;
    }

    result->integer *= sign;

    return result;
}

PARSER(parse) {
    if (buffer_len(ctx) == 0 || CHAR <= 32)
        return new_error("empty buffer");

    if (CHAR == '-' || CHAR == '+')
        CALL_PARSER(parse_integer);

    if (is_digit(CHAR))
        CALL_PARSER(parse_integer);

    if (is_valid_identifier(CHAR))
        CALL_PARSER(parse_symbol);

    return new_error("no parser found for input");
}

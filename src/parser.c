#include <stdlib.h>
#include <stdio.h>

#include "parser.h"

void free_result(ParserResult* result) {
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
    strlen(ctx->buffer);
}

void free_ctx(ParserContext* ctx) {
    if (ctx->buffer)
        free(ctx->buffer);

    free(ctx);
}

int is_digit(char c) {
    return c >= '0' && c <= '9';
}

PARSER(parse_integer) {
    int sign = 0;

    if (*ctx->buffer == '-') sign = -1;
    else sign = 1;

    // Offset by sign.
    if (!is_digit(*ctx->buffer))
        NEXT;

    int number = 0;

    ITERATE_WORD {
        if (!is_digit(*ctx->buffer))
            ERROR("Expected a digit");

        int digit = *ctx->buffer - '0';
        number = number * 10 + digit;

        NEXT;
    }

    number *= sign;

    RESULT(integer, number);
}

PARSER(parse) {
    if (*ctx->buffer == '\0')
        ERROR("Empty buffer");

    if (*ctx->buffer == '-' || *ctx->buffer == '+')
        return parse_integer(ctx);

    if (is_digit(*ctx->buffer))
        return parse_integer(ctx);

    ERROR("No parser found for input");
}

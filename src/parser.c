#include <stdlib.h>
#include <stdio.h>

#include "parser.h"

struct ParserResult* new_result() {
    struct ParserResult* result = malloc(sizeof(struct ParserResult));

    result->integer = NULL;
    result->symbol = NULL;
    result->error = NULL;
    result->next = NULL;
    result->list = NULL;

    return result;
}

struct ParserResult* new_error(const char* message) {
    struct ParserResult* result = new_result();
    result->error = message;
    return result;
}

void free_result(struct ParserResult* result) {
    if (result->symbol)
        free(result->symbol);

    if (result->integer)
        free(result->integer);

    if (result->next)
        free_result(result->next);

    if (result->list)
        free_result(result->list);

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

void print_result(struct ParserContext* ctx, struct ParserResult* result, int depth) {
    if (result->error) {
        for (int i = 0; i < ctx->position + 2; i++)
            putchar(' ');
        printf("^ %s\n", result->error);

        return;
    }

    while (result) {
        if (result->list) {
            print_result(ctx, result->list, depth + 1);
        } else {
            for (int i = 0; i < depth; i++)
                putchar('\t');

            if (result->symbol)
                printf("sym: %s", result->symbol);
            else if (result->integer)
                printf("int: %d", *result->integer);
            else
                printf("nil");

            putchar('\n');
        }

        result = result->next;
    }
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

PARSER(parse_list) {
    if (*ctx->at == '(')
        next(ctx);
    else PASS;

    if (*ctx->at == ')')
        return new_result();

    struct ParserResult* result = new_result();
    struct ParserResult* start = NULL;
    struct ParserResult* node = NULL;

    while (ctx->position < buffer_len(ctx)) {
        if (*ctx->at == ')') {
            next(ctx);
            result->list = start;
            return result;
        }

        struct ParserResult* parsed = parse_token(ctx);

        if (parsed->error) {
            free_result(result);
            return new_error(parsed->error);
        }

        if (start) {
            node->next = parsed;
            node = node->next;
        } else {
            start = parsed;
            node = start;
        }
    }

    free_result(result);
    return new_error("end of file");
}

PARSER(parse_symbol) {
    if (!is_valid_identifier(*ctx->at))
        PASS;

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
    else if (*ctx->at == '+') sign = 1;
    else if (is_digit(*ctx->at)) sign = 1;
    else PASS;

    if (!is_digit(*ctx->at)) {
        if (!is_digit(ctx->at[1]))
            PASS;
        next(ctx);
    }

    struct ParserResult* result = new_result();
    result->integer = malloc(sizeof(int));
    *result->integer = 0;

    OVER_WORD {
        if (!is_digit(*ctx->at)) {
            free_result(result);
            return new_error("expected a digit");
        }

        int digit = *ctx->at - '0';
        *result->integer = *result->integer * 10 + digit;

        next(ctx);
    }

    *result->integer *= sign;

    return result;
}

#define CALL_PARSER(parser)                             \
    do {                                                \
        struct ParserResult* result = parser(ctx);      \
        if (result) return result;                      \
    } while (0)

PARSER(parse_token) {
    while (*ctx->at <= ' ' && *ctx->at != '\0')
        next(ctx);

    if (buffer_len(ctx) == 0 || *ctx->at == '\0')
        return new_error("empty buffer");

    CALL_PARSER(parse_integer);
    CALL_PARSER(parse_symbol);
    CALL_PARSER(parse_list);

    return new_error("no parser found for input");
}

PARSER(parse) {
    struct ParserResult* token = parse_token(ctx);

    if (token->error)
        return token;

    if (ctx->position < buffer_len(ctx) - 1) {
        free_result(token);
        return new_error("trailing characters");
    }

    return token;
}

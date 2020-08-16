#include <stdlib.h>
#include <stdio.h>

#include "parse.h"

static struct Value* STATIC = NULL;

struct ListNode* new_list_node() {
    struct ListNode* node = malloc(sizeof(struct ListNode));

    node->value = NULL;
    node->next = NULL;

    return node;
}

void free_list_node(struct ListNode* node, struct Value* owner) {
    if (node->value)
        free_owned_value(node->value, owner);

    if (node->next)
        free_list_node(node->next, owner);

    free(node);
}

struct Value* new_value() {
    struct Value* value = malloc(sizeof(struct Value));

    value->integer = NULL;
    value->symbol = NULL;
    value->quoted = 0;
    value->list = NULL;
    value->error = NULL;
    value->lifetime = NULL;

    return value;
}

struct Value* new_error(const char* message) {
    struct Value* value = new_value();
    value->error = message;
    return value;
}

void make_static(struct Value* value) {
    if (!STATIC) {
        STATIC = new_value();
        STATIC->lifetime = STATIC;
    }

    value->lifetime = STATIC;
}

void set_ownership(struct Value* value, struct Value* owner) {
    if (value->lifetime != STATIC)
        value->lifetime = owner;
}

void set_quoted(struct Value* value, bool quoted) {
    if (value->list) {
        value->quoted = quoted;

        if (quoted) {
            struct ListNode* node = value->list;

            while (node && node->value) {
                set_quoted(node->value, quoted);
                node = node->next;
            }
        }
    } else if (!value->integer) {
        value->quoted = quoted;
    }
}

void free_owned_value(struct Value* value, struct Value* owner) {
    if (value->lifetime == STATIC || value->lifetime != owner)
        return;

    if (value->symbol)
        free(value->symbol);

    if (value->integer)
        free(value->integer);

    if (value->list)
        free_list_node(value->list, value);

    free(value);
}

void free_value(struct Value* value) {
    free_owned_value(value, NULL);
}

struct Context* new_ctx() {
    struct Context* ctx = malloc(sizeof(struct Context));

    ctx->size = 1024;
    ctx->buffer = calloc(ctx->size, sizeof(char));
    ctx->at = ctx->buffer;
    ctx->position = 0;

    return ctx;
}

void next(struct Context* ctx) {
    ctx->at++;
    ctx->position++;
}

void prev(struct Context* ctx) {
    ctx->at--;
    ctx->position--;
}

size_t buffer_len(struct Context* ctx) {
    return strlen(ctx->buffer);
}

void free_ctx(struct Context* ctx) {
    if (ctx->buffer)
        free(ctx->buffer);

    free(ctx);
}

void print_value(struct Context* ctx, struct Value* value, int depth) {
    if (value->error) {
        for (int i = 0; i < ctx->position + 2; i++)
            putchar(' ');
        printf("^ %s\n", value->error);

        return;
    }

    for (int i = 0; i < depth; i++)
        putchar('\t');

    if (value->list) {
        printf("q: %d; list:\n", value->quoted);

        struct ListNode* node = value->list;

        while (node && node->value) {
            print_value(ctx, node->value, depth + 1);
            node = node->next;
        }
    } else if (value->symbol) {
        printf("sym: %s; q: %d\n", value->symbol, value->quoted);
    } else if (value->integer) {
        printf("int: %d\n", *value->integer);
    } else {
        printf("nil\n");
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
    bool quoted = false;

    if (*ctx->at == '\'') {
        quoted = true;
        next(ctx);
    }

    if (*ctx->at == '(')
        next(ctx);
    else if (quoted) {
        prev(ctx);
        PASS;
    } else PASS;

    if (*ctx->at == ')') {
        next(ctx);
        return new_value();
    }

    struct Value* value = new_value();
    struct ListNode* list = new_list_node();

    value->list = list;

    while (ctx->position < buffer_len(ctx)) {
        if (*ctx->at == ')') {
            next(ctx);
            set_quoted(value, quoted);
            return value;
        }

        struct Value* result = parse_token(ctx);

        if (result->error) {
            free_value(value);
            return result;
        }

        set_ownership(result, value);

        list->value = result;
        list->next = new_list_node();
        list = list->next;
    }

    free_value(value);
    return new_error("end of file");
}

PARSER(parse_symbol) {
    int quoted = 0;

    if (*ctx->at == '\'') {
        quoted = 1;
        next(ctx);
    }

    if (!is_valid_identifier(*ctx->at)) {
        if (quoted) prev(ctx);
        PASS;
    }

    struct Value* value = new_value();
    value->symbol = calloc(ctx->size, sizeof(char));

    size_t index = 0;

    OVER_WORD {
        if (!is_valid_identifier(*ctx->at)) {
            free_value(value);
            return new_error("not a valid identifier");
        }

        value->symbol[index++] = *ctx->at;

        next(ctx);
    }

    set_quoted(value, quoted);

    return value;
}

PARSER(parse_integer) {
    int sign = 0;

    if (*ctx->at == '-') sign = -1;
    else if (*ctx->at == '+' || is_digit(*ctx->at)) sign = 1;
    else PASS;

    if (!is_digit(ctx->at[0])) {
        if (!is_digit(ctx->at[1]))
            PASS;
        next(ctx);
    }

    struct Value* value = new_value();
    value->integer = malloc(sizeof(int));
    *value->integer = 0;

    OVER_WORD {
        if (!is_digit(*ctx->at)) {
            free_value(value);
            return new_error("expected a digit");
        }

        int digit = *ctx->at - '0';
        *value->integer = *value->integer * 10 + digit;

        next(ctx);
    }

    *value->integer *= sign;

    return value;
}

#define CALL_PARSER(parser)                     \
    do {                                        \
        struct Value* value = parser(ctx);      \
        if (value) return value;                \
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
    struct Value* token = parse_token(ctx);

    if (token->error)
        return token;

    if (ctx->position < buffer_len(ctx) - 1) {
        free_value(token);
        return new_error("trailing characters");
    }

    return token;
}

struct Value* quick_parse(const char* buffer) {
    struct Context* ctx = new_ctx();
    strcpy(ctx->buffer, buffer);

    struct Value* parsed = parse(ctx);

    free_ctx(ctx);

    return parsed;
}

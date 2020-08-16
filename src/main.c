#include <stdio.h>
#include <stdlib.h>

#include "eval.h"

int main(int argc, char** argv) {
    put_symbol("answer", quick_parse("42"));
    put_symbol("true", quick_parse("1"));
    put_symbol("false", quick_parse("0"));

    for (;;) {
        struct Context* ctx = new_ctx();

        fprintf(stderr, "> ");

        if (getline(&ctx->buffer, &ctx->size, stdin) == EOF)
            break;

        struct Value* value = eval(parse(ctx));
        print_value(ctx, value, 0);

        free_ctx(ctx);
        free_value(value);
    }

    return 0;
}

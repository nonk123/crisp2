#include <stdio.h>
#include <stdlib.h>

#include "parser.h"

int main(int argc, char** argv) {
    for (;;) {
        struct Context* ctx = new_ctx();

        printf("> ");
        getline(&ctx->buffer, &ctx->size, stdin);

        struct Value* value = parse(ctx);
        print_value(ctx, value, 0);

        free_value(value);
        free_ctx(ctx);
    }

    return 0;
}

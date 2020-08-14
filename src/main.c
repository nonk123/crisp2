#include <stdio.h>
#include <stdlib.h>

#include "parse.h"

int main(int argc, char** argv) {
    for (;;) {
        struct Context* ctx = new_ctx();

        printf("> ");

        if (getline(&ctx->buffer, &ctx->size, stdin) == EOF) {
            putchar('\n');
            break;
        }

        struct Value* value = parse(ctx);
        print_value(ctx, value, 0);

        free_ctx(ctx);
        free_value(value);
    }

    return 0;
}

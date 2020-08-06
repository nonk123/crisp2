#include <stdio.h>
#include <stdlib.h>

#include "parser.h"

int main(int argc, char** argv) {
    for (;;) {
        struct ParserContext* ctx = new_ctx();

        printf("> ");
        getline(&ctx->buffer, &ctx->size, stdin);

        struct ParserResult* result = parse(ctx);
        print_result(ctx, result, 0);

        free_result(result);
        free_ctx(ctx);
    }

    return 0;
}

#include <stdio.h>
#include <stdlib.h>

#include "parser.h"

int main(int argc, char** argv) {
    for (;;) {
        ParserContext* ctx = new_ctx();

        printf("> ");
        getline(&ctx->buffer, &ctx->size, stdin);

        ParserResult* result = parse(ctx);

        if (result->error) {
            for (int i = 0; i < ctx->position; i++)
                printf(" ");
            printf("  ^ %s", result->error);
        } else if (result->integer) {
            printf("%d", result->integer);
        }

        putchar('\n');
    }

    return 0;
}

#include <stdio.h>
#include <stdlib.h>

#include "parser.h"

int main(int argc, char** argv) {
    const char* ps1 = "> ";

    for (;;) {
        struct ParserContext* ctx = new_ctx();

        printf("%s", ps1);
        getline(&ctx->buffer, &ctx->size, stdin);

        struct ParserResult* result = parse(ctx);

        if (result->error) {
            for (int i = 0; i < ctx->position + strlen(ps1); i++)
                printf(" ");
            printf("^ %s", result->error);
        } else if (result->symbol) {
            printf("sym: %s", result->symbol);
        } else {
            printf("int: %d", result->integer);
        }

        putchar('\n');

        free_result(result);
        free_ctx(ctx);
    }

    return 0;
}

#include <stdio.h>
#include <stdlib.h>

#include "parser.h"

int main(int argc, char** argv) {
    for (;;) {
        size_t position = 0;
        size_t size = 1024;
        char* buffer = calloc(size, sizeof(char));

        printf("> ");
        getline(&buffer, &size, stdin);

        ParserResult result = parse(&position, buffer, size);

        if (result.error) {
            for (int i = 0; i < position; i++)
                printf(" ");
            printf("  ^ %s\n", result.error);
        } else if (result.integer)
            printf("%d\n", *result.integer);
    }

    return 0;
}

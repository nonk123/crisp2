#include <stdio.h>
#include <stdlib.h>

#include "parser.h"

int main(int argc, char** argv) {
    for (;;) {
        size_t size = 1024;
        char* buffer = calloc(size, sizeof(char));

        printf("> ");
        getline(&buffer, &size, stdin);

        ParserResult result = parse(buffer, size);

        if (result.error)
            printf("%s\n", result.error);
        else if (result.integer)
            printf("%d\n", *result.integer);
    }

    return 0;
}

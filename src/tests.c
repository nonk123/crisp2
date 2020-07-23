#define TEST(name) PARSER(name)
#define RUN_TEST(name) name(&position, buffer, size)

#define ASSERT(message, expr) do {                      \
        if (!(expr))                                    \
            printf("Test failed: %s\n", message);       \
        else                                            \
            printf("Test succeeded: %s\n", message);    \
    } while (0)
#define ASSERT_PARSE(expression, field, expected) do {          \
        buffer = expression;                                    \
        ParserResult result = CALL_PARSER(parse);               \
        ASSERT((expression), *result.field == expected);        \
    } while (0)
#define ASSERT_ERROR(expression) do {                   \
        buffer = expression;                            \
        ParserResult result = CALL_PARSER(parse);       \
        ASSERT((expression), result.error);             \
    } while (0)

#include <stdio.h>
#include <stdlib.h>

#include "parser.h"

TEST(integer_test) {
    ASSERT_PARSE("+1000", integer,  1000);
    ASSERT_PARSE("-1000", integer, -1000);
    ASSERT_PARSE( "1000", integer,  1000);
    ASSERT_ERROR( "1aaa");
}

int main(int argc, char** argv) {
    size_t position = 0;
    size_t size = 1024;
    char* buffer = calloc(size, sizeof(char));

    RUN_TEST(integer_test);

    printf("Testing complete.\n");

    return 0;
}

#define TEST(name) int name()
#define RUN_TEST(name)                                  \
    do {                                                \
        if (name())                                     \
            printf("Test failed: %s\n\n", #name);       \
        else                                            \
            printf("Test successful: %s\n\n", #name);   \
    } while (0)

#define ASSERT(buffer, expression)                              \
    do {                                                        \
        if (!(expression)) {                                    \
            printf("FAILED %s\t=> %s\n", buffer, #expression);  \
            return 1;                                           \
        } else {                                                \
            printf("  OK   %s\t=> %s\n", buffer, #expression);  \
        }                                                       \
    } while (0)
#define ASSERT_PARSE(_buffer, expression)       \
    do {                                        \
        ParserContext* ctx = new_ctx();         \
        strcpy(ctx->buffer, _buffer);           \
        ParserResult* result = parse(ctx);      \
        ASSERT(_buffer, expression);            \
    } while (0)
#define ASSERT_EQ(buffer, field, expected)              \
    ASSERT_PARSE(buffer, result->field == expected)
#define ASSERT_ERROR(buffer)                            \
    ASSERT_PARSE(buffer, !result || result->error)
#define ASSERT_INT(_integer)                                    \
    ASSERT_PARSE(#_integer, result->integer == _integer)
#define ASSERT_SYM(_symbol)                                     \
    ASSERT_PARSE(_symbol, !strcmp(result->symbol, _symbol))

#define SUCCESS return 0;

#include <stdio.h>
#include <stdlib.h>

#include "parser.h"

TEST(integer_test) {
    ASSERT_INT(12345);
    ASSERT_INT(+1000);
    ASSERT_INT(-1000);
    ASSERT_INT( 1000);
    ASSERT_INT(   +0);
    ASSERT_INT(   -0);
    ASSERT_INT(    0);
    ASSERT_ERROR( "1aaa");
    ASSERT_ERROR("+1aaa");
    ASSERT_ERROR("-1aaa");
    SUCCESS;
}

TEST(symbol_test) {
    ASSERT_SYM("-car");
    ASSERT_SYM("+hello+");
    ASSERT_SYM("*world*");
    ASSERT_ERROR("-1a");
    SUCCESS;
}

int main(int argc, char** argv) {
    RUN_TEST(integer_test);
    RUN_TEST(symbol_test);

    printf("All tests complete\n");

    return 0;
}

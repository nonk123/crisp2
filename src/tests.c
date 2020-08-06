#define TEST(name) int name()

#define ASSERT(buffer, expression)                              \
    do {                                                        \
        if (!(expression)) {                                    \
            printf("FAILED %s\t=> %s\n", buffer, #expression);  \
            return 1;                                           \
        } else {                                                \
            printf("  OK   %s\t=> %s\n", buffer, #expression);  \
        }                                                       \
    } while (0)
#define WITH_PARSED(_buffer, body)                      \
    do {                                                \
        struct ParserContext* ctx = new_ctx();          \
        strcpy(ctx->buffer, _buffer);                   \
        struct ParserResult* result = parse(ctx);       \
        body;                                           \
        free_result(result);                            \
        free_ctx(ctx);                                  \
    } while (0)

#define ASSERT_PARSE(_buffer, expression)       \
    WITH_PARSED(_buffer, ASSERT(_buffer, expression))
#define ASSERT_EQ(buffer, field, expected)              \
    ASSERT_PARSE(buffer, result->field == expected)
#define ASSERT_ERROR(buffer)                            \
    ASSERT_PARSE(buffer, !result || result->error)
#define ASSERT_INT(_integer)                                    \
    ASSERT_PARSE(#_integer, *result->integer == _integer)
#define ASSERT_SYM(_symbol)                                     \
    ASSERT_PARSE(_symbol, !strcmp(result->symbol, _symbol))
#define WITH_LIST(buffer, body)                         \
    WITH_PARSED(buffer, do {                            \
            struct ParserResult* list = result->list;   \
            body;                                       \
        } while (0))

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
    SUCCESS;
}

TEST(list_test) {
    // TODO: add more tests.
    WITH_LIST("(1 2)", do {
            ASSERT("(1 2)", *list->integer == 1);
            ASSERT("(1 2)", *list->next->integer == 2);
        } while (0));
    SUCCESS;
}

TEST(errors_test) {
    ASSERT_ERROR("123a");
    ASSERT_ERROR("-1a");
    SUCCESS;
}

#define RUN_TEST(name)                                  \
    do {                                                \
        total++;                                        \
                                                        \
        if (name())                                     \
            printf("Test failed: %s\n\n", #name);       \
        else {                                          \
            successful++;                               \
            printf("Test successful: %s\n\n", #name);   \
        }                                               \
    } while (0)

int main(int argc, char** argv) {
    int successful = 0;
    int total = 0;

    RUN_TEST(errors_test);
    RUN_TEST(integer_test);
    RUN_TEST(symbol_test);
    RUN_TEST(list_test);

    printf("All tests complete\nSuccess rate: %d/%d\n", successful, total);

    return successful != total;
}

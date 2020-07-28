#define TEST(name) void name()
#define RUN_TEST(name) name()

#define ASSERT(buffer, expression)                              \
    do {                                                        \
        if (!(expression)) {                                    \
            printf("[FAILED] %s => %s\n", buffer, #expression); \
            exit(1);                                            \
        } else {                                                \
            printf("[  OK  ] %s => %s\n", buffer, #expression); \
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
#define ASSERT_SYM(buffer)                                      \
    ASSERT_PARSE(buffer, !strcmp(result->symbol, buffer))

#include <stdio.h>
#include <stdlib.h>

#include "parser.h"

TEST(integer_test) {
    ASSERT_EQ("+1000", integer,  1000);
    ASSERT_EQ("-1000", integer, -1000);
    ASSERT_EQ( "1000", integer,  1000);
    ASSERT_ERROR( "1aaa");
}

TEST(symbol_test) {
    ASSERT_SYM("-car");
    ASSERT_SYM("+hello+");
    ASSERT_SYM("*world*");
    ASSERT_ERROR("-1a");
}

int main(int argc, char** argv) {
    RUN_TEST(integer_test);
    RUN_TEST(symbol_test);

    printf("Testing complete. All clear.\n");

    return 0;
}

#define TEST(name) void name()
#define RUN_TEST(name) name()

#define ASSERT(expression) do {                         \
        if (!(expression)) {                            \
            printf("[FAILED] %s\n", #expression);       \
            exit(1);                                    \
        } else {                                        \
            printf("[  OK  ] %s\n", #expression);       \
        }                                               \
    } while (0)
#define ASSERT_PARSE(_buffer, expression) do {  \
        ParserContext* ctx = new_ctx();         \
        strcpy(ctx->buffer, _buffer);           \
        ParserResult* result = parse(ctx);      \
        ASSERT(expression);                     \
    } while (0)
#define ASSERT_EQ(buffer, field, expected)              \
    ASSERT_PARSE(buffer, result->field == expected)
#define ASSERT_ERROR(buffer)                            \
    ASSERT_PARSE(buffer, !result || result->error)

#include <stdio.h>
#include <stdlib.h>

#include "parser.h"

TEST(integer_test) {
    ASSERT_EQ("+1000", integer,  1000);
    ASSERT_EQ("-1000", integer, -1000);
    ASSERT_EQ( "1000", integer,  1000);
    ASSERT_ERROR( "1aaa");
}

int main(int argc, char** argv) {
    RUN_TEST(integer_test);

    printf("Testing complete.\n");

    return 0;
}

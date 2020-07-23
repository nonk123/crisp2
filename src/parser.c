#include <stdlib.h>
#include <stdio.h>

#include "parser.h"

int is_digit(char c) {
    return c >= '0' && c <= '9';
}

PARSER(parse_integer) {
    int sign = 0;

    if (buffer[0] == '-') sign = -1;
    else sign = 1;

    // Offset by sign.
    if (!is_digit(buffer[0])) {
        NEXT;
    }

    ALLOC(number, int, 0);

    ITERATE_WORD {
        if (!is_digit(*buffer))
            RESULT(.error, "Expected a digit");

        int digit = *buffer - '0';
        *number = *number * 10 + digit;

        NEXT;
    }

    *number *= sign;

    RESULT(.integer, number);
}

PARSER(parse) {
    if (!buffer || size == 0)
        RESULT(.error, "Empty buffer");

    if (buffer[0] == '-' || buffer[0] == '+' && size > 1)
        return CALL_PARSER(parse_integer);

    if (is_digit(buffer[0]))
        return CALL_PARSER(parse_integer);

    RESULT(.error, "No parser found for input");
}

#include <stdlib.h>
#include <stdio.h>

#include "parser.h"

ParserResult error(const char* message) {
    ParserResult result = { .error = message };
    return result;
}

int is_digit(char c) {
    return c >= '0' && c <= '9';
}

ParserResult parse_integer(char* buffer, size_t size) {
    int sign = 0;

    if (buffer[0] == '-') sign = -1;
    else sign = 1;

    // Offset by sign.
    buffer += is_digit(buffer[0]) ? 0 : 1;

    int* number = malloc(sizeof(int));
    *number = 0;

    // Stop when a control character is hit.
    while (*buffer >= 32) {
        if (!is_digit(*buffer))
            return error("Not a number");

        int digit = *buffer++ - '0';
        *number = *number * 10 + digit;
    }

    *number *= sign;

    ParserResult result = {.integer = number};
    return result;
}

ParserResult parse(char* buffer, size_t size) {
    if (!buffer || size == 0)
        return error("Empty buffer");

    if (buffer[0] == '-' || buffer[0] == '+' && size > 1)
        return parse_integer(buffer, size);

    if (is_digit(buffer[0]))
        return parse_integer(buffer, size);

    return error("No parser found for input");
}

#ifndef PARSER_H
#define PARSER_H

#include <stddef.h>

typedef struct {
    int* integer;
    const char* error;
} ParserResult;

ParserResult error(const char* message);

int is_digit(char c);

ParserResult parse_int(char* buffer, size_t size);

ParserResult parse(char* buffer, size_t size);

#endif

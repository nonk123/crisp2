#ifndef PARSER_H
#define PARSER_H

#include <stddef.h>

#define PARSER(name) ParserResult name(int* position, char* buffer, size_t size)
#define ITERATE(condition) while (condition)
#define ITERATE_WORD while(*buffer > 32)
#define NEXT do { *position += 1; buffer++; } while (0)
#define ALLOC(variable, type, value) type * variable = malloc(sizeof(type)); \
    *variable = value
#define RESULT(field, value) do { ParserResult _result = {field = value}; \
        return _result; } while (0)
#define CALL_PARSER(name) name(position, buffer, size)

typedef struct {
    int* integer;
    const char* error;
    } ParserResult;

int is_digit(char c);

PARSER(parse_int);
PARSER(parse);

#endif

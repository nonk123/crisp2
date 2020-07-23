#ifndef PARSER_H
#define PARSER_H

typedef struct {
    int* integer;
    const char* error;
} ParserResult;

ParserResult parse(char* buffer);

#endif

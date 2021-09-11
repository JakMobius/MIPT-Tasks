

#ifndef BONKSCRIPT_PARSER_H
#define BONKSCRIPT_PARSER_H

#include <stdlib.h>
#include <stdbool.h>

typedef struct compiler s_compiler;
typedef struct parser s_parser;
typedef struct parser_position s_parser_position;

#include "../tree.h"
#include "./grammatics/grammatic_global.h"
#include "../compiler.h"

typedef struct parser_position {
    int line;
    int ch;
    unsigned long index;
    const char* filename;
} s_parser_position;

typedef struct parser {
    const char* text;
    s_parser_position position;
    s_compiler* compiler;
    s_parser_position* position_stack;
    int position_stack_capacity;
    int position_stack_size;
} s_parser;

char parser_next_char(s_parser* parser);
void parser_eat_char();

s_parser* parser_create(s_compiler* compiler);
void parser_init(s_parser* parser, s_compiler* compiler);
void parser_destruct(s_parser* parser);
void parser_release(s_parser* parser);

s_tree* parser_parse_file(s_parser* parser, const char* filename, const char* text);

bool parser_save_position(s_parser* parser);
bool parser_restore_position(s_parser* parser);

#endif //BONKSCRIPT_PARSER_H



#ifndef BONKSCRIPT_BONK_PARSER_H
#define BONKSCRIPT_BONK_PARSER_H

#include <stdlib.h>
#include <stdbool.h>

typedef struct bonk_parser s_bonk_parser;

#include "../tree.h"
#include "./grammatics/bonk_grammatic_global.h"
#include "../bonk_compiler.h"
#include "./lexic/bonk_lexical_analyzer.h"
#include "../bonk_scope_stack.h"

typedef struct bonk_parser {
    unsigned long position;
    s_bonk_lexeme* input;
    s_bonk_compiler* compiler;
    unsigned long* position_stack;
    int position_stack_capacity;
    int position_stack_size;
} s_bonk_parser;

s_bonk_lexeme* bonk_parser_next_lexeme(s_bonk_parser* parser);
void bonk_parser_eat_lexeme(s_bonk_parser* parser);
void bonk_parser_spit_lexeme(s_bonk_parser* parser);

s_bonk_parser* bonk_parser_create(s_bonk_compiler* compiler);
void bonk_parser_init(s_bonk_parser* parser, s_bonk_compiler* compiler);
void bonk_parser_destruct(s_bonk_parser* parser);
void bonk_parser_release(s_bonk_parser* parser);

s_tree* bonk_parser_parse_file(s_bonk_parser* parser, s_bonk_lexeme* lexemes);
bool bonk_parser_save_position(s_bonk_parser* parser);
bool bonk_parser_restore_position(s_bonk_parser* parser);

#endif //BONKSCRIPT_BONK_PARSER_H


#ifndef BONKSCRIPT_BONK_LEXICAL_ANALYZER_H
#define BONKSCRIPT_BONK_LEXICAL_ANALYZER_H

typedef struct bonk_lexeme s_bonk_lexeme;
typedef struct bonk_lexical_analyzer s_bonk_lexical_analyzer;
typedef struct bonk_parser_position s_bonk_parser_position;

typedef enum bonk_lexeme_type {
    BONK_LEXEME_NULL,
    BONK_LEXEME_KEYWORD,
    BONK_LEXEME_IDENTIFIER,
    BONK_LEXEME_NUMBER,
    BONK_LEXEME_SEMICOLON,
    BONK_LEXEME_OPERATOR,
    BONK_LEXEME_BRACE,
    BONK_LEXEME_COMMA,
    BONK_LEXEME_CALL,
    BONK_LEXEME_INLINE_BAMS
} e_bonk_lexeme_type;

typedef enum bonk_keyword_type {
    BONK_KEYWORD_VAR,
    BONK_KEYWORD_BLOCK,
    BONK_KEYWORD_PRINT,
    BONK_KEYWORD_CONTEXT,
    BONK_KEYWORD_BONK,
    BONK_KEYWORD_CHECK,
    BONK_KEYWORD_CYCLE,
    BONK_KEYWORD_BREK,
    BONK_KEYWORD_BAMS,
    BONK_KEYWORD_AND,
    BONK_KEYWORD_OR,
    BONK_KEYWORD_REBONK,
    BONK_KEYWORD_HELP
} e_bonk_keyword_type;

typedef enum bonk_brace_type {
    BONK_BRACE_L_CB,
    BONK_BRACE_R_CB,
    BONK_BRACE_L_RB,
    BONK_BRACE_R_RB,
    BONK_BRACE_L_SB,
    BONK_BRACE_R_SB
} e_bonk_brace_type;

typedef enum bonk_operator_type {
    BONK_OPERATOR_PLUS,
    BONK_OPERATOR_MINUS,
    BONK_OPERATOR_MULTIPLY,
    BONK_OPERATOR_DIVIDE,
    BONK_OPERATOR_ASSIGNMENT,
    BONK_OPERATOR_EQUALS,
    BONK_OPERATOR_NOT_EQUAL,
    BONK_OPERATOR_LESS_THAN,
    BONK_OPERATOR_GREATER_THAN,
    BONK_OPERATOR_LESS_OR_EQUAL_THAN,
    BONK_OPERATOR_GREATER_OR_EQUAL_THAN,
    BONK_OPERATOR_CYCLE,
    BONK_OPERATOR_CHECK,
    BONK_OPERATOR_CALL,
    BONK_OPERATOR_PRINT,
    BONK_OPERATOR_BONK,
    BONK_OPERATOR_BREK,
    BONK_OPERATOR_BAMS,
    BONK_OPERATOR_AND,
    BONK_OPERATOR_OR,
    BONK_OPERATOR_REBONK
} e_bonk_operator_type;

#include <ctype.h>
#include "../bonk_parser.h"
#include "../../bonk_compiler.h"
#include "bonk_identifier_lexeme.h"
#include "bonk_number_lexeme.h"

extern const char* BONK_OPERATOR_NAMES[];
extern const char* BONK_KEYWORD_NAMES[];

typedef struct bonk_lexeme {
    s_bonk_parser_position* position;
    e_bonk_lexeme_type type;

    union {
        struct {
            e_bonk_keyword_type keyword_type;
        } keyword_data;
        struct {
            e_bonk_brace_type brace_type;
        } brace_data;
        struct {
            s_tree_node_number* number;
        } number_data;
        struct {
            s_tree_node_identifier* identifier;
        } identifier_data;
        struct {
            e_bonk_operator_type operator_type;
        } operator_data;
    };
} s_bonk_lexeme;

typedef struct bonk_parser_position {
    const char* filename;
    unsigned long line;
    unsigned long ch;
    unsigned long index;
} s_bonk_parser_position;

typedef struct bonk_lexical_analyzer {
    const char* text;
    s_bonk_parser_position position;
    s_bonk_compiler* compiler;
    s_bonk_lexeme* lexemes;
    unsigned long lexemes_buffer_capacity;
    unsigned long lexemes_amount;
    bool is_line_comment;
    bool is_multiline_comment;
} s_bonk_lexical_analyzer;

void bonk_lexical_analyzer_init(s_bonk_lexical_analyzer* analyzer, s_bonk_compiler* compiler);
void bonk_lexical_analyzer_destruct(s_bonk_lexical_analyzer* analyzer);
s_bonk_lexical_analyzer* bonk_lexical_analyzer_create(s_bonk_compiler* compiler);
void bonk_lexical_analyzer_release(s_bonk_lexical_analyzer* analyzer);
s_bonk_lexeme* bonk_lexical_analyzer_parse_file(s_bonk_lexical_analyzer* analyzer, const char* filename, const char* text);
char bonk_lexical_analyzer_next_char(s_bonk_lexical_analyzer* analyzer);
void bonk_lexical_analyzer_eat_char(s_bonk_lexical_analyzer* analyzer);

s_bonk_parser_position* bonk_parser_position_clone(s_bonk_parser_position* position);

#endif //BONKSCRIPT_BONK_LEXICAL_ANALYZER_H

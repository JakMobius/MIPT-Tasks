
#ifndef BONKSCRIPT_BONK_LEXICAL_ANALYZER_H
#define BONKSCRIPT_BONK_LEXICAL_ANALYZER_H

namespace bonk {

struct lexeme;
struct lexical_analyzer;
struct parser_position;

enum lexeme_type {
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
};

enum keyword_type {
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
    BONK_KEYWORD_HELP,
    BONK_KEYWORD_PROMISE
};

enum brace_type {
    BONK_BRACE_L_CB,
    BONK_BRACE_R_CB,
    BONK_BRACE_L_RB,
    BONK_BRACE_R_RB,
    BONK_BRACE_L_SB,
    BONK_BRACE_R_SB
};

}

#include <ctype.h>
#include "../parser.hpp"
#include "../../compiler.hpp"
#include "identifier_lexeme.hpp"
#include "number_lexeme.hpp"
#include "../parser_position.hpp"
#include "../../tree/nodes/nodes.hpp"

namespace bonk {

extern const char* BONK_OPERATOR_NAMES[];
extern const char* BONK_KEYWORD_NAMES[];

struct lexeme {
    parser_position* position;
    lexeme_type type;

    union {
        struct {
            keyword_type keyword_type;
        } keyword_data;
        struct {
            brace_type brace_type;
        } brace_data;
        struct {
            tree_node_number* number;
        } number_data;
        struct {
            tree_node_identifier* identifier;
        } identifier_data;
        struct {
            operator_type operator_type;
        } operator_data;
    };
};

struct lexical_analyzer {
    const char* text;
    parser_position position;
    compiler* linked_compiler;
    dynamic_array<lexeme>* lexemes;
    dynamic_array<const char*> compiled_files;
    bool is_line_comment;
    bool is_multiline_comment;

    void error(const char *format, ...);

    static lexical_analyzer* create(compiler* compiler);
    void construct(compiler* compiler);
    void destruct();
    void release();
    dynamic_array<bonk::lexeme>* parse_file(const char* filename, const char* text);
    char next_char();
    void eat_char();

    bool parse_number_lexeme(lexeme* target);
    int parse_digits_lexeme(int radix, long long int* integer_value, double* float_value);
    bool parse_identifier_lexeme(lexeme* target);

    void make_operator_lexeme(lexeme* lexeme, operator_type type);

    void make_brace_lexeme(lexeme* lexeme, brace_type type);

    keyword_type keyword_from_string(const char* string, unsigned long length);

    bool add_compiled_file(const char* file_path);
    bool file_already_compiled(const char* file_path);

    bool next();
};

}

#endif //BONKSCRIPT_BONK_LEXICAL_ANALYZER_H

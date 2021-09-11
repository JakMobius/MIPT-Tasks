
#include "bonk_lexical_analyzer.h"

const char* BONK_KEYWORD_NAMES[] = {
    "var",
    "block",
    "print",
    "context",
    "bonk",
    "check",
    "cycle",
    "brek",
    "bams",
    "and",
    "or",
    "rebonk",
    "help",
    NULL
};

const char* BONK_OPERATOR_NAMES[] = {
    "+",
    "-",
    "*",
    "/",
    "=",
    "==",
    "<",
    ">",
    "<=",
    ">=",
    "!=",
    "",
    NULL
};

s_bonk_lexical_analyzer* bonk_lexical_analyzer_create(s_bonk_compiler* compiler) {
    s_bonk_lexical_analyzer* analyzer = calloc(1, sizeof(s_bonk_lexical_analyzer));
    if(!analyzer) return NULL;
    bonk_lexical_analyzer_init(analyzer, compiler);
    return analyzer;
}

void bonk_lexical_analyzer_init(s_bonk_lexical_analyzer* analyzer, s_bonk_compiler* compiler) {
    if(!analyzer) return;

    analyzer->compiler = compiler;
    analyzer->lexemes_buffer_capacity = 0;
}

void bonk_lexical_analyzer_destruct(s_bonk_lexical_analyzer* analyzer) {
    if(!analyzer) return;
}

void bonk_lexical_analyzer_release(s_bonk_lexical_analyzer* analyzer) {
    bonk_lexical_analyzer_destruct(analyzer);
    free(analyzer);
}

void bonk_lexical_analyzer_add_lexeme(s_bonk_lexical_analyzer* analyzer, s_bonk_lexeme* lexeme) {
    if(analyzer->lexemes_amount == analyzer->lexemes_buffer_capacity) {
        if(analyzer->lexemes_buffer_capacity == 0) {
            analyzer->lexemes_buffer_capacity = 128;
        } else {
            analyzer->lexemes_buffer_capacity *= 2;
        }

        s_bonk_lexeme* new_buffer = realloc(analyzer->lexemes, analyzer->lexemes_buffer_capacity * sizeof(s_bonk_lexeme));
        if(!new_buffer) {
            compiler_out_of_memory(analyzer->compiler);
            return;
        }

        analyzer->lexemes = new_buffer;
    }

    if(!lexeme) {
        memset(analyzer->lexemes + analyzer->lexemes_amount++, 0, sizeof(s_bonk_lexeme));
    } else {
        memcpy(analyzer->lexemes + analyzer->lexemes_amount++, lexeme, sizeof(s_bonk_lexeme));
    }
}

void bonk_make_operator_lexeme(s_bonk_lexeme* lexeme, e_bonk_operator_type type) {
    lexeme->type = BONK_LEXEME_OPERATOR;
    lexeme->operator_data.operator_type = type;
}

void bonk_make_brace_lexeme(s_bonk_lexeme* lexeme, e_bonk_brace_type type) {
    lexeme->type = BONK_LEXEME_BRACE;
    lexeme->brace_data.brace_type = type;
}

bool bonk_lexical_analyzer_next(s_bonk_lexical_analyzer* analyzer) {
    char c = bonk_lexical_analyzer_next_char(analyzer);

    if(c == '\0') {
        return false;
    }

    if(analyzer->is_line_comment) {
        if(c == '\n') {
            analyzer->is_line_comment = false;
        }
        bonk_lexical_analyzer_eat_char(analyzer);
        return true;
    }

    if(analyzer->is_multiline_comment) {
        if(c == '*') {
            bonk_lexical_analyzer_eat_char(analyzer);
            c = bonk_lexical_analyzer_next_char(analyzer);
            if(c == '/') analyzer->is_multiline_comment = false;
            else return true;
        }
        bonk_lexical_analyzer_eat_char(analyzer);
        return true;
    }

    if(c == ' ' || c == '\n' || c == '\t') {
        bonk_lexical_analyzer_eat_char(analyzer);
        return true;
    }

    s_bonk_lexeme next_lexeme = {0};
    s_bonk_parser_position position = analyzer->position;

    if(c == '@') {
        next_lexeme.type = BONK_LEXEME_CALL;
        bonk_lexical_analyzer_eat_char(analyzer);
    } else if(c == ';') {
        next_lexeme.type = BONK_LEXEME_SEMICOLON;
        bonk_lexical_analyzer_eat_char(analyzer);
    } else if(c == ',') {
        next_lexeme.type = BONK_LEXEME_COMMA;
        bonk_lexical_analyzer_eat_char(analyzer);
    } else if(isalpha(c)) {
        if(!bonk_parse_identifier_lexeme(analyzer, &next_lexeme)) return false;
    } else if(isdigit(c)) {
        if(!bonk_parse_number_lexeme(analyzer, &next_lexeme)) return false;
    } else if(c == '/') {
        bonk_lexical_analyzer_eat_char(analyzer);
        char next = bonk_lexical_analyzer_next_char(analyzer);

        if(next == '/') {
            analyzer->is_line_comment = true;
            bonk_lexical_analyzer_eat_char(analyzer);
            return true;
        }
        else if(next == '*') {
            analyzer->is_multiline_comment = true;
            bonk_lexical_analyzer_eat_char(analyzer);
            return true;
        }
        else bonk_make_operator_lexeme(&next_lexeme, BONK_OPERATOR_DIVIDE);
    } else if(c == '=' || c == '>' || c == '<') {
        bonk_lexical_analyzer_eat_char(analyzer);
        if(bonk_lexical_analyzer_next_char(analyzer) == '=') {
            switch(c) {
                case '<': bonk_make_operator_lexeme(&next_lexeme, BONK_OPERATOR_LESS_OR_EQUAL_THAN); break;
                case '>': bonk_make_operator_lexeme(&next_lexeme, BONK_OPERATOR_GREATER_OR_EQUAL_THAN); break;
                default: bonk_make_operator_lexeme(&next_lexeme, BONK_OPERATOR_EQUALS); break;
            }
            bonk_lexical_analyzer_eat_char(analyzer);
        } else {
            switch(c) {
                case '<': bonk_make_operator_lexeme(&next_lexeme, BONK_OPERATOR_LESS_THAN); break;
                case '>': bonk_make_operator_lexeme(&next_lexeme, BONK_OPERATOR_GREATER_THAN); break;
                default: bonk_make_operator_lexeme(&next_lexeme, BONK_OPERATOR_ASSIGNMENT); break;
            }
        }
    } else if(c == '!') {
        bonk_lexical_analyzer_eat_char(analyzer);
        if(bonk_lexical_analyzer_next_char(analyzer) == '=') {
            bonk_lexical_analyzer_eat_char(analyzer);
            bonk_make_operator_lexeme(&next_lexeme, BONK_OPERATOR_NOT_EQUAL);
        } else {
            bonk_lexical_error(analyzer, "use 'not' instead of '!'");
            return false;
        }
    } else if(c == '{') {
        bonk_lexical_analyzer_eat_char(analyzer);
        if(analyzer->lexemes_amount == 0) {
            bonk_make_brace_lexeme(&next_lexeme, BONK_BRACE_L_CB);
        } else {
            s_bonk_lexeme* last_lexeme = analyzer->lexemes + analyzer->lexemes_amount - 1;
            if(last_lexeme->type != BONK_LEXEME_KEYWORD ||
               last_lexeme->keyword_data.keyword_type != BONK_KEYWORD_BAMS) {
                bonk_make_brace_lexeme(&next_lexeme, BONK_BRACE_L_CB);
            } else {
                next_lexeme.type = BONK_LEXEME_INLINE_BAMS;
                next_lexeme.identifier_data.identifier = tree_create_node_identifier();

                if(!next_lexeme.identifier_data.identifier) {
                    compiler_out_of_memory(analyzer->compiler);
                    return false;
                }

                unsigned long begin_position = analyzer->position.index;
                next_lexeme.identifier_data.identifier->variable_name = analyzer->text + analyzer->position.index;

                while(true) {
                    c = bonk_lexical_analyzer_next_char(analyzer);
                    if(c == '\0') {
                        bonk_lexical_error(analyzer, "Expected ede code");
                        return false;
                    }
                    bonk_lexical_analyzer_eat_char(analyzer);
                    if(c == '}') {
                        break;
                    }
                }

                next_lexeme.identifier_data.identifier->name_length = analyzer->position.index - begin_position - 1;
            }
        }
    } else {
        switch(c) {
            case '+': bonk_make_operator_lexeme(&next_lexeme, BONK_OPERATOR_PLUS); break;
            case '-': bonk_make_operator_lexeme(&next_lexeme, BONK_OPERATOR_MINUS); break;
            case '*': bonk_make_operator_lexeme(&next_lexeme, BONK_OPERATOR_MULTIPLY); break;
            case '}': bonk_make_brace_lexeme(&next_lexeme, BONK_BRACE_R_CB); break;
            case '[': bonk_make_brace_lexeme(&next_lexeme, BONK_BRACE_L_SB); break;
            case ']': bonk_make_brace_lexeme(&next_lexeme, BONK_BRACE_R_SB); break;
            case '(': bonk_make_brace_lexeme(&next_lexeme, BONK_BRACE_L_RB); break;
            case ')': bonk_make_brace_lexeme(&next_lexeme, BONK_BRACE_R_RB); break;
            default: bonk_lexical_error(analyzer, "Expected lexeme"); return false;
        }
        bonk_lexical_analyzer_eat_char(analyzer);
    }

    s_bonk_parser_position* lexeme_position = calloc(1, sizeof(s_bonk_parser_position));
    memcpy(lexeme_position, &position, sizeof(s_bonk_parser_position));
    next_lexeme.position = lexeme_position;

    bonk_lexical_analyzer_add_lexeme(analyzer, &next_lexeme);

    return true;
}

s_bonk_lexeme* bonk_lexical_analyzer_parse_file(s_bonk_lexical_analyzer* analyzer, const char* filename, const char* text) {

    if(bonk_compiler_file_already_compiled(analyzer->compiler, filename)) {
        return NULL;
    }

    bonk_compiler_add_compiled_file(analyzer->compiler, filename);

    analyzer->position.filename = filename;
    analyzer->position.index = 0;
    analyzer->position.ch = 1;
    analyzer->position.line = 1;
    analyzer->text = text;
    analyzer->lexemes_amount = 0;
    analyzer->lexemes_buffer_capacity = 0;

    while(!analyzer->compiler->state) {
        if(!bonk_lexical_analyzer_next(analyzer)) break;
    }

    bonk_lexical_analyzer_add_lexeme(analyzer, NULL);
    analyzer->lexemes[analyzer->lexemes_amount - 1].position = bonk_parser_position_clone(&analyzer->position);

    s_bonk_lexeme* lexemes = analyzer->lexemes;

    analyzer->lexemes = NULL;

    return lexemes;
}

char bonk_lexical_analyzer_next_char(s_bonk_lexical_analyzer* analyzer) {
    return analyzer->text[analyzer->position.index];
}

void bonk_lexical_analyzer_eat_char(s_bonk_lexical_analyzer* analyzer) {
    char c = bonk_lexical_analyzer_next_char(analyzer);

    assert(c != '\0');

    analyzer->position.ch++;
    analyzer->position.index++;
    if(c == '\n') {
        analyzer->position.line++;
        analyzer->position.ch = 1;
    }
}

s_bonk_parser_position* bonk_parser_position_clone(s_bonk_parser_position* position) {
    if(position == NULL) return NULL;
    s_bonk_parser_position* new_position = calloc(1, sizeof(*position));

    new_position->index = position->index;
    new_position->line = position->line;
    new_position->ch = position->ch;

    // TODO: использовать таблицу имен файлов в структуре компилятора

    new_position->filename = strdup(position->filename);

    return new_position;
}
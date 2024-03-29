
#include "lexical_analyzer.hpp"

namespace bonk {

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
    "promise",
    nullptr
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
    nullptr
};

lexical_analyzer* lexical_analyzer::create(compiler* compiler) {
    auto* analyzer = (lexical_analyzer*)calloc(1, sizeof(lexical_analyzer));
    if(!analyzer) return nullptr;
    analyzer->construct(compiler);
    return analyzer;
}

void lexical_analyzer::construct(compiler* compiler) {
    linked_compiler = compiler;
    compiled_files.construct(4);
}

void lexical_analyzer::destruct() {
    for(int i = 0; i < compiled_files.size; i++) {
        free((void*) compiled_files.get(i));
    }
    compiled_files.destruct();
}

void lexical_analyzer::release() {
    destruct();
    free(this);
}

void lexical_analyzer::make_operator_lexeme(lexeme* lexeme, operator_type type) {
    lexeme->type = BONK_LEXEME_OPERATOR;
    lexeme->operator_data.operator_type = type;
}

void lexical_analyzer::make_brace_lexeme(lexeme* lexeme, brace_type type) {
    lexeme->type = BONK_LEXEME_BRACE;
    lexeme->brace_data.brace_type = type;
}

bool lexical_analyzer::next() {
    char c = next_char();

    if(c == '\0') {
        return false;
    }

    if(is_line_comment) {
        if(c == '\n') {
            is_line_comment = false;
        }
        eat_char();
        return true;
    }

    if(is_multiline_comment) {
        if(c == '*') {
            eat_char();
            c = next_char();
            if(c == '/') is_multiline_comment = false;
            else return true;
        }
        eat_char();
        return true;
    }

    if(c == ' ' || c == '\n' || c == '\t') {
        eat_char();
        return true;
    }

    parser_position saved_position = position;
    lexeme next_lexeme = {};
    next_lexeme.position = &saved_position;

    if(c == '@') {
        next_lexeme.type = BONK_LEXEME_CALL;
        eat_char();
    } else if(c == ';') {
        next_lexeme.type = BONK_LEXEME_SEMICOLON;
        eat_char();
    } else if(c == ',') {
        next_lexeme.type = BONK_LEXEME_COMMA;
        eat_char();
    } else if(isalpha(c)) {
        if(!parse_identifier_lexeme(&next_lexeme)) return false;
    } else if(isdigit(c)) {
        if(!parse_number_lexeme(&next_lexeme)) return false;
    } else if(c == '/') {
        eat_char();
        char next = next_char();

        if(next == '/') {
            is_line_comment = true;
            eat_char();
            return true;
        }
        else if(next == '*') {
            is_multiline_comment = true;
            eat_char();
            return true;
        }
        else make_operator_lexeme(&next_lexeme, BONK_OPERATOR_DIVIDE);
    } else if(c == '=' || c == '>' || c == '<') {
        eat_char();
        if(next_char() == '=') {
            switch(c) {
                case '<': make_operator_lexeme(&next_lexeme, BONK_OPERATOR_LESS_OR_EQUAL_THAN); break;
                case '>': make_operator_lexeme(&next_lexeme, BONK_OPERATOR_GREATER_OR_EQUAL_THAN); break;
                default: make_operator_lexeme(&next_lexeme, BONK_OPERATOR_EQUALS); break;
            }
            eat_char();
        } else {
            switch(c) {
                case '<': make_operator_lexeme(&next_lexeme, BONK_OPERATOR_LESS_THAN); break;
                case '>': make_operator_lexeme(&next_lexeme, BONK_OPERATOR_GREATER_THAN); break;
                default: make_operator_lexeme(&next_lexeme, BONK_OPERATOR_ASSIGNMENT); break;
            }
        }
    } else if(c == '!') {
        eat_char();
        if(next_char() == '=') {
            eat_char();
            make_operator_lexeme(&next_lexeme, BONK_OPERATOR_NOT_EQUAL);
        } else {
            error("use 'not' instead of '!'");
            return false;
        }
    } else if(c == '{') {
        eat_char();
        if(lexemes->size == 0) {
            make_brace_lexeme(&next_lexeme, BONK_BRACE_L_CB);
        } else {
            lexeme last_lexeme = lexemes->get(lexemes->size - 1);
            if(last_lexeme.type != BONK_LEXEME_KEYWORD ||
               last_lexeme.keyword_data.keyword_type != BONK_KEYWORD_BAMS) {
                make_brace_lexeme(&next_lexeme, BONK_BRACE_L_CB);
            } else {
                next_lexeme.type = BONK_LEXEME_INLINE_BAMS;

                unsigned long begin_position = position.index;
                const char* begin_pointer = text + begin_position;

                while(true) {
                    c = next_char();
                    if(c == '\0') {
                        error("Expected inline assembly");
                        return false;
                    }
                    eat_char();
                    if(c == '}') {
                        break;
                    }
                }

                unsigned long length = position.index - begin_position - 1;
                auto* inline_assembly = tree_node_identifier::create(begin_pointer, length);

                if(!inline_assembly) {
                    linked_compiler->out_of_memory();
                    return false;
                }

                next_lexeme.identifier_data.identifier = inline_assembly;
            }
        }
    } else {
        switch(c) {
            case '+': make_operator_lexeme(&next_lexeme, BONK_OPERATOR_PLUS); break;
            case '-': make_operator_lexeme(&next_lexeme, BONK_OPERATOR_MINUS); break;
            case '*': make_operator_lexeme(&next_lexeme, BONK_OPERATOR_MULTIPLY); break;
            case '}': make_brace_lexeme(&next_lexeme, BONK_BRACE_R_CB); break;
            case '[': make_brace_lexeme(&next_lexeme, BONK_BRACE_L_SB); break;
            case ']': make_brace_lexeme(&next_lexeme, BONK_BRACE_R_SB); break;
            case '(': make_brace_lexeme(&next_lexeme, BONK_BRACE_L_RB); break;
            case ')': make_brace_lexeme(&next_lexeme, BONK_BRACE_R_RB); break;
            default: error("Expected lexeme"); return false;
        }
        eat_char();
    }

    next_lexeme.position = next_lexeme.position->clone();

    lexemes->push(next_lexeme);

    return true;
}

dynamic_array<lexeme>* lexical_analyzer::parse_file(const char* filename, const char* filetext) {

    if(file_already_compiled(filename)) {
        return nullptr;
    }

    filename = strdup(filename);

    add_compiled_file(filename);

    position.filename = filename;
    position.index = 0;
    position.ch = 1;
    position.line = 1;
    text = filetext;
    lexemes = dynamic_array<lexeme>::create();

    while(!linked_compiler->state) {
        if(!next()) break;
    }

    dynamic_array<lexeme>* file_lexemes = lexemes;
    file_lexemes->push({});

    lexemes = nullptr;

    return file_lexemes;
}

char lexical_analyzer::next_char() {
    return text[position.index];
}

void lexical_analyzer::eat_char() {
    char c = next_char();

    assert(c != '\0');

    position.ch++;
    position.index++;
    if(c == '\n') {
        position.line++;
        position.ch = 1;
    }
}

bool lexical_analyzer::add_compiled_file(const char* file_path) {
    compiled_files.push(file_path);
    return true;
}

bool lexical_analyzer::file_already_compiled(const char* file_path) {
    for(int i = 0; i < compiled_files.size; i++) {
        if(strcmp(compiled_files.get(i), file_path) == 0) return true;
    }
    return false;
}

}
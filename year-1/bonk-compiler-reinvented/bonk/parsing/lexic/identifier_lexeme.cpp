
/*
 * Identifier lexeme
 * $IDENTIFIER = [a-zA-Z][a-zA-Z0-9]+
 */

#include "identifier_lexeme.hpp"

namespace bonk {

bool lexical_analyzer::parse_identifier_lexeme(lexeme* target) {
    const char* name_start = text + position.index;

    parser_position identifier_position = position;

    while(true) {
        char c = next_char();
        if(isdigit(c)) {
            if(position.index == identifier_position.index) {
                break;
            }
        } else if(!isalpha(c) && c != '_') break;

        eat_char();
    }

    if(identifier_position.index == position.index) return false;

    keyword_type keyword = keyword_from_string(name_start, position.index - identifier_position.index);
    if(keyword != (keyword_type)-1) {
        target->type = BONK_LEXEME_KEYWORD;
        target->keyword_data.keyword_type = keyword;
        return true;
    }

    unsigned long length = position.index - identifier_position.index;

    tree_node_identifier* identifier = tree_node_identifier::create(name_start, length);

    if(!identifier) {
        linked_compiler->out_of_memory();
        return false;
    }

    identifier->source_position = identifier_position.clone();

    target->type = BONK_LEXEME_IDENTIFIER;
    target->identifier_data.identifier = identifier;
    return true;
}

keyword_type lexical_analyzer::keyword_from_string(const char* string, unsigned long length) {
    for(int keyword = 0; BONK_KEYWORD_NAMES[(keyword_type)keyword]; keyword++) {
        if(strncmp(string, BONK_KEYWORD_NAMES[keyword], length) == 0) {
            if(BONK_KEYWORD_NAMES[keyword][length] == '\0') return (keyword_type)keyword;
        }
    }
    return (keyword_type)-1;
}

}
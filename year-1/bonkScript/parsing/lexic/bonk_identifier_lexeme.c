
/*
 * Identifier lexeme
 * $IDENTIFIER = [a-zA-Z][a-zA-Z0-9]+
 */

#include "bonk_identifier_lexeme.h"

bool bonk_parse_identifier_lexeme(s_bonk_lexical_analyzer* analyzer, s_bonk_lexeme* target) {
    const char* name_start = analyzer->text + analyzer->position.index;

    s_bonk_parser_position identifier_position = analyzer->position;

    while(true) {
        char c = bonk_lexical_analyzer_next_char(analyzer);
        if(isdigit(c)) {
            if(analyzer->position.index == identifier_position.index) {
                break;
            }
        } else if(!isalpha(c) && c != '_') break;

        bonk_lexical_analyzer_eat_char(analyzer);
    }

    if(identifier_position.index == analyzer->position.index) return false;

    e_bonk_keyword_type keyword = bonk_keyword_from_string(name_start, analyzer->position.index - identifier_position.index);
    if(keyword != -1) {
        target->type = BONK_LEXEME_KEYWORD;
        target->keyword_data.keyword_type = keyword;
        return true;
    }

    s_tree_node_identifier* identifier = tree_create_node_identifier();
    if(!identifier) {
        compiler_out_of_memory(analyzer->compiler);
        return false;
    }
    identifier->variable_name = name_start;
    identifier->name_length = analyzer->position.index - identifier_position.index;
    identifier->tree_node.source_position = bonk_parser_position_clone(&identifier_position);

    target->type = BONK_LEXEME_IDENTIFIER;
    target->identifier_data.identifier = identifier;
    return true;
}

e_bonk_keyword_type bonk_keyword_from_string(const char* string, unsigned long length) {
    for(e_bonk_keyword_type keyword = 0; BONK_KEYWORD_NAMES[keyword]; keyword++) {
        if(strncmp(string, BONK_KEYWORD_NAMES[keyword], length) == 0) {
            if(BONK_KEYWORD_NAMES[keyword][length] == '\0') return keyword;
        }
    }
    return -1;
}
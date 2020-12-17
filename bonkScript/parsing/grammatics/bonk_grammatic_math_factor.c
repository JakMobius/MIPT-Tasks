/*
 * Math factor grammatic:
 * $MATH_FACTOR := $REFERENCE|$NUMBER
 */

#include "bonk_grammatic_math_factor.h"

s_tree_node* bonk_parse_grammatic_math_factor(s_bonk_parser* parser) {
    s_tree_node* result = NULL;

    result = bonk_parse_grammatic_reference(parser);
    if(result) return result;

    if(bonk_parser_next_lexeme(parser)->type == BONK_LEXEME_NUMBER) {
        result = (s_tree_node*) bonk_parser_next_lexeme(parser)->number_data.number;
        bonk_parser_eat_lexeme(parser);
        return result;
    }

    return NULL;
}
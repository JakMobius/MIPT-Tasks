/*
 * Math expression grammatic:
 * $MATH_EXPRESSION := $MATH_TERM (['+'|'-']($MATH_TERM))*
 */

#include "bonk_grammatic_math_expression.h"

s_tree_node* bonk_parse_grammatic_math_expression(s_bonk_parser* parser) {
    unsigned long old_position = parser->position;
    s_tree_node* term = bonk_parse_grammatic_math_term(parser);
    if(!term && old_position != parser->position) return NULL;

    s_tree_node* result = term;

    do {
        s_bonk_lexeme* next = bonk_parser_next_lexeme(parser);
        if(next->type != BONK_LEXEME_OPERATOR) break;
        e_bonk_operator_type operator_type = next->operator_data.operator_type;
        if(operator_type != BONK_OPERATOR_PLUS && operator_type != BONK_OPERATOR_MINUS) break;

        bonk_parser_eat_lexeme(parser);

        s_tree_node* next_term = bonk_parse_grammatic_math_term(parser);
        if(!next_term) {
            if(!parser->compiler->state) {
                bonk_parser_error(parser, "expected expression");
            }
            return NULL;
        }

        s_tree_node* sum = (s_tree_node*)tree_create_node_operator(operator_type);
        if(!sum) {
            compiler_out_of_memory(parser->compiler);
            return NULL;
        }
        sum->left = result;
        sum->right = next_term;
        result = sum;

    } while(true);

    return result;
}
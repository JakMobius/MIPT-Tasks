/*
 * Comparation grammatic:
 * $MATH_FACTOR := $MATH_EXPRESSION ($COMPARATION_OPERATOR $MATH_EXPRESSION)*
 */

#include "bonk_grammatic_comparation.h"

s_tree_node* bonk_parse_grammatic_comparation(s_bonk_parser* parser) {

    s_tree_node* expression = bonk_parse_grammatic_math_expression(parser);
    if(!expression) return NULL;

    s_tree_node* result = expression;

    do {
        s_bonk_lexeme* next = bonk_parser_next_lexeme(parser);
        if(next->type != BONK_LEXEME_OPERATOR) break;
        e_bonk_operator_type operator_type = next->operator_data.operator_type;
        if(!bonk_is_comparation_operator(operator_type)) break;

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

bool bonk_is_comparation_operator(e_bonk_operator_type operator) {
    switch(operator) {
        case BONK_OPERATOR_EQUALS:
        case BONK_OPERATOR_LESS_THAN:
        case BONK_OPERATOR_GREATER_THAN:
        case BONK_OPERATOR_LESS_OR_EQUAL_THAN:
        case BONK_OPERATOR_GREATER_OR_EQUAL_THAN:
        case BONK_OPERATOR_NOT_EQUAL:
            return true;
        default:
            return false;
    }
}
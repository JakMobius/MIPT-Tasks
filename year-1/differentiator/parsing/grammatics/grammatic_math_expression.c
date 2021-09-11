/*
 * Math expression grammatic:
 * $MATH_EXPRESSION := 'd'? $MATH_TERM (['+'|'-']($MATH_TERM))*
 */

#include "grammatic_math_expression.h"

s_tree_node* parse_grammatic_math_expression(s_parser* parser) {
    unsigned long old_position = parser->position.index;
    s_tree_node* term = parse_grammatic_math_term(parser);
    if(!term && old_position != parser->position.index) return NULL;

    s_tree_node* result = term;

    do {
        char c = parser_next_char(parser);
        if(c != '+' && c != '-') break;

        parser_eat_char(parser);

        s_tree_node* next_term = parse_grammatic_math_term(parser);
        if(!next_term) {
            if(!parser->compiler->state) {
                parser_error(parser, "expected expression");
            }
            return NULL;
        }

        s_tree_node* sum = (s_tree_node*)tree_create_node_operator(c == '+' ? TREE_OPERATOR_TYPE_PLUS : TREE_OPERATOR_TYPE_MINUS);
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
/*
 * Math multiplication grammatic:
 * $MATH_TERM := $MATH_FACTOR (['*']($MATH_FACTOR))*
 */

#include "grammatic_math_term.h"

s_tree_node* parse_grammatic_math_term(s_parser* parser) {

    s_tree_node* term = parse_grammatic_math_factor(parser);
    s_tree_node* result = term;

    do {
        char c = parser_next_char(parser);
        if(c != '*' && c != '/') break;
        parser_eat_char(parser);

        s_tree_node* next_term = parse_grammatic_math_factor(parser);
        if(!next_term) {
            if(!parser->compiler->state) {
                parser_error(parser, "expected expression");
            }
            return NULL;
        }

        s_tree_node* sum = (s_tree_node*) tree_create_node_operator(c == '*' ? TREE_OPERATOR_TYPE_MULTIPLY : TREE_OPERATOR_TYPE_DIVIDE);
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
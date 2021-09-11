/*
 * Expression grammatic:
 * $EXPRESSION := $MATH_EXPRESSION
 */

#include "grammatic_expression.h"

s_tree_node* parse_grammatic_expression(s_parser* parser) {
    return parse_grammatic_math_expression(parser);
}
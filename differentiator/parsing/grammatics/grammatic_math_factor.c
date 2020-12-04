/*
 * Math factor grammatic:
 * $MATH_FACTOR := $REFERENCE|$NUMBER
 */

#include "grammatic_math_factor.h"

s_tree_node* parse_grammatic_math_factor(s_parser* parser) {

    bool is_differential = false;

    if(parser_next_char(parser) == 'd') {
        is_differential = true;
        parser_eat_char(parser);
    }

    s_tree_node* factor = parse_grammatic_power_base(parser);
    if(factor == NULL) {
        if(is_differential) {
            factor = (s_tree_node*) tree_create_node_operator(TREE_OPERATOR_TYPE_DIFFERENTIAL);
            if(!factor) compiler_out_of_memory(parser->compiler);
            return factor;
        }
        return NULL;
    }

    if(parser_next_char(parser) == '^') {
        parser_eat_char(parser);
        s_tree_node* power = parse_grammatic_power_base(parser);
        if(!power) {
            parser_error(parser, "expected power indicator");
            free(factor);
            return NULL;
        }

        s_tree_node* base = factor;

        factor = (s_tree_node*) tree_create_node_operator(TREE_OPERATOR_TYPE_POW);
        if(!factor) {
            free(base);
            free(power);
            compiler_out_of_memory(parser->compiler);
            return NULL;
        }
        factor->left = base;
        factor->right = power;
    }

    if(is_differential) {
        s_tree_node* differential = (s_tree_node*) tree_create_node_operator(TREE_OPERATOR_TYPE_DIFFERENTIAL);
        if(!differential) {
            compiler_out_of_memory(parser->compiler);
            tree_release_subtree(factor);
            return NULL;
        }
        differential->right = factor;
        return differential;
    }

    return factor;
}
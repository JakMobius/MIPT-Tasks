/*
 * Expression grammatic:
 * $EXPRESSION := $ASSIGNMENT | $LOGIC_EXPRESSION | $UNARY_OPERATOR
 */

#include "bonk_grammatic_expression.h"

s_tree_node* bonk_parse_grammatic_expression(s_bonk_parser* parser) {
    return bonk_parse_grammatic_expression_leveled(parser, false);
}

s_tree_node* bonk_parse_grammatic_expression_leveled(s_bonk_parser* parser, bool top_level) {

    s_tree_node* expression = bonk_parse_grammatic_logic_expression(parser);
    if(parser->compiler->state) return NULL;

    if(!expression) {
        expression = bonk_parse_grammatic_unary_operator(parser);
        if(parser->compiler->state) return NULL;
    }

    if(top_level) {

        if(!expression) {
            expression = bonk_parse_grammatic_var_definition(parser);
            if(parser->compiler->state) return NULL;
        }

        s_bonk_lexeme* next = bonk_parser_next_lexeme(parser);
        if(next->type != BONK_LEXEME_SEMICOLON) {
            if(expression) {
                bonk_parser_error(parser, "expected semicolon");
            }
        } else {
            while(next->type == BONK_LEXEME_SEMICOLON) {
                bonk_parser_eat_lexeme(parser);
                next = bonk_parser_next_lexeme(parser);
            }
            if(!expression) {
                return bonk_parse_grammatic_expression(parser);
            }
        }
    }

    if(!expression) {
        expression = bonk_parse_grammatic_sub_block(parser);
        if(parser->compiler->state) return NULL;
    }

    return expression;
}
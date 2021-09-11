/*
 * Assignment grammatic
 * $ASSIGNMENT := $IDENTIFIER '=' $EXPRESSION
 */

#include "bonk_grammatic_assignment.h"

s_tree_node* bonk_parse_grammatic_assignment(s_bonk_parser* parser) {

    s_bonk_lexeme* lvalue = bonk_parser_next_lexeme(parser);
    if(lvalue->type == BONK_LEXEME_NULL) return NULL;

    bonk_parser_eat_lexeme(parser);
    s_bonk_lexeme* assignment = bonk_parser_next_lexeme(parser);

    if(assignment->type != BONK_LEXEME_OPERATOR || assignment->operator_data.operator_type != BONK_OPERATOR_ASSIGNMENT) {
        // This is not an assignment. Exit silently.
        bonk_parser_spit_lexeme(parser);
        return NULL;
    }

    if(lvalue->type != BONK_LEXEME_IDENTIFIER) {
        bonk_compiler_error_positioned(parser->compiler, lvalue->position, "expression is not assignable");
        return NULL;
    }

    bonk_parser_eat_lexeme(parser);

    s_tree_node* rvalue = bonk_parse_grammatic_expression(parser);
    if(!rvalue) {
        if(parser->compiler->state) return NULL;
        bonk_parser_error(parser, "expected value");
        return NULL;
    }

    s_tree_node* expression = (s_tree_node*) tree_create_node_operator(BONK_OPERATOR_ASSIGNMENT);

    expression->left = (s_tree_node*) lvalue->identifier_data.identifier;
    expression->right = rvalue;

    return expression;
}
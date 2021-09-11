
/*
 * Var definition grammatic
 * Var_Definition { "var" Identifier }
 */
#include "bonk_grammatic_var_definition.h"

s_tree_node_variable_definition* bonk_parse_grammatic_var_definition(s_bonk_parser* parser) {

    s_bonk_lexeme* next = bonk_parser_next_lexeme(parser);
    bool is_contextual = false;

    if(next->type != BONK_LEXEME_KEYWORD) return NULL;

    if(next->keyword_data.keyword_type == BONK_KEYWORD_CONTEXT) {
        is_contextual = true;

        bonk_parser_eat_lexeme(parser);
        next = bonk_parser_next_lexeme(parser);
        if(next->type != BONK_LEXEME_KEYWORD) return NULL;
    }

    if(next->keyword_data.keyword_type != BONK_KEYWORD_VAR) return NULL;

    bonk_parser_eat_lexeme(parser);
    next = bonk_parser_next_lexeme(parser);

    if(next->type != BONK_LEXEME_IDENTIFIER) {
        bonk_parser_error(parser, "expected variable name");
        return NULL;
    }

    bonk_parser_eat_lexeme(parser);

    s_tree_node_variable_definition * definition = tree_create_node_variable_definition();
    definition->is_contextual = is_contextual;
    definition->tree_node.left = (s_tree_node*) next->identifier_data.identifier;

    definition->tree_node.source_position = bonk_parser_position_clone(next->position);

    next = bonk_parser_next_lexeme(parser);

    if(next->type == BONK_LEXEME_OPERATOR && next->operator_data.operator_type == BONK_OPERATOR_ASSIGNMENT) {
        bonk_parser_eat_lexeme(parser);
        if(is_contextual) {
            bonk_parser_error(parser, "context variable may not be initialized");
            tree_node_release((s_tree_node*) definition);
            return NULL;
        }

        definition->tree_node.right = bonk_parse_grammatic_expression(parser);

        if(definition->tree_node.right == NULL) {
            tree_node_release((s_tree_node*) definition);
            if(!parser->compiler->state) {
                bonk_parser_error(parser, "expected initial variable value");
            }
            return NULL;
        }
    }

    return (s_tree_node_variable_definition*) definition;
}
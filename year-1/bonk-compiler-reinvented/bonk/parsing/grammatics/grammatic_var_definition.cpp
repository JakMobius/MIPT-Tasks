
/*
 * Var definition grammatic
 * Var_Definition { "var" Identifier }
 */
#include "grammatic_var_definition.hpp"

namespace bonk {

tree_node_variable_definition* parse_grammatic_var_definition(parser* parser) {

    lexeme* next = parser->next_lexeme();
    bool is_contextual = false;

    if(next->type != BONK_LEXEME_KEYWORD) return nullptr;

    if(next->keyword_data.keyword_type == BONK_KEYWORD_CONTEXT) {
        is_contextual = true;

        parser->eat_lexeme();
        next = parser->next_lexeme();
        if(next->type != BONK_LEXEME_KEYWORD) return nullptr;
    }

    if(next->keyword_data.keyword_type != BONK_KEYWORD_VAR) return nullptr;

    parser->eat_lexeme();
    next = parser->next_lexeme();

    if(next->type != BONK_LEXEME_IDENTIFIER) {
        parser->error("expected variable name");
        return nullptr;
    }

    parser->eat_lexeme();

    tree_node_variable_definition* definition = tree_node_variable_definition::create(is_contextual, next->identifier_data.identifier);

    definition->source_position = next->position->clone();

    next = parser->next_lexeme();

    if(next->type == BONK_LEXEME_OPERATOR && next->operator_data.operator_type == BONK_OPERATOR_ASSIGNMENT) {
        parser->eat_lexeme();
        if(is_contextual) {
            parser->error("context variable may not be initialized");
            definition->release();
            return nullptr;
        }

        definition->variable_value = parse_grammatic_expression(parser);

        if(definition->variable_value == nullptr) {
            definition->release();
            if(!parser->linked_compiler->state) {
                parser->error("expected initial variable value");
            }
            return nullptr;
        }
    }

    return definition;
}

}
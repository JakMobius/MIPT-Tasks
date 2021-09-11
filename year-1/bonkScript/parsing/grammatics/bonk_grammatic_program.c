/*
 * Program grammatic
 * $PROGRAM = ($BLOCK_DEFINITION | $VAR_DEFINITION)+
 */

#include "bonk_grammatic_program.h"

s_tree_node* bonk_parse_grammatic_global_definition(s_bonk_parser* parser) {
    s_tree_node* expression = bonk_parse_grammatic_block_definition(parser);
    if(expression) return expression;
    if(parser->compiler->state) return NULL;

    s_tree_node_variable_definition* variable_definition = bonk_parse_grammatic_var_definition(parser);
    if(variable_definition) {
        if(variable_definition->is_contextual) {
            bonk_parser_error(parser, "global variables may not be contextual");
            tree_node_release((s_tree_node*) variable_definition);
            return NULL;
        }
        s_bonk_lexeme* next = bonk_parser_next_lexeme(parser);
        if(next->type != BONK_LEXEME_SEMICOLON) {
            bonk_parser_error(parser, "expected semicolon");
            tree_node_release((s_tree_node*) variable_definition);
            return NULL;
        }
        bonk_parser_eat_lexeme(parser);
        return (s_tree_node*) variable_definition;
    }

    return NULL;
}

s_tree_node* bonk_parse_grammatic_program(s_bonk_parser* parser) {
    s_tree_node* list = tree_create_typed_node(TREE_NODE_TYPE_LIST);

    if(!list) {
        compiler_out_of_memory(parser->compiler);
        return NULL;
    }

    s_tree_node* result = list;

    s_tree_node* block = bonk_parse_grammatic_global_definition(parser);
    if(!block) {
        tree_release_subtree(list);
        return NULL;
    }
    while(block) {
        list->left = block;
        list->right = tree_create_typed_node(TREE_NODE_TYPE_LIST);

        if(!list->right) {
            compiler_out_of_memory(parser->compiler);
            return NULL;
        }
        list = list->right;

        block = bonk_parse_grammatic_global_definition(parser);
    }

    return result;
}
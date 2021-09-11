/*
 * Block definition grammatic
 * $BLOCK_DEFINITION = 'block' $IDENTIFIER '{' $BLOCK '}'
 */

#include "bonk_grammatic_block_definition.h"

s_tree_node* bonk_parse_grammatic_block_definition(s_bonk_parser* parser) {

    s_bonk_lexeme* next = bonk_parser_next_lexeme(parser);

    if(next->type != BONK_LEXEME_KEYWORD) return NULL;
    if(next->keyword_data.keyword_type != BONK_KEYWORD_BLOCK) return NULL;

    bonk_parser_eat_lexeme(parser);
    next = bonk_parser_next_lexeme(parser);

    if(next->type != BONK_LEXEME_IDENTIFIER) {
        bonk_parser_error(parser, "expected block name");
        return NULL;
    }

    s_tree_node_identifier* block_identifier = next->identifier_data.identifier;

    bonk_parser_eat_lexeme(parser);
    next = bonk_parser_next_lexeme(parser);

    if(next->type != BONK_LEXEME_BRACE || next->brace_data.brace_type != BONK_BRACE_L_CB) {
        bonk_parser_error(parser, "expected '{'");
        tree_release_subtree((s_tree_node*) block_identifier);
        return NULL;
    }

    bonk_parser_eat_lexeme(parser);

    s_tree_node* block = bonk_parse_grammatic_block(parser);
    if(!block) {
        tree_release_subtree((s_tree_node*) block_identifier);
        return NULL;
    }

    next = bonk_parser_next_lexeme(parser);

    if(next->type != BONK_LEXEME_BRACE || next->brace_data.brace_type != BONK_BRACE_R_CB) {
        bonk_parser_error(parser, "expected '}'");
        tree_release_subtree(block);
        tree_release_subtree((s_tree_node*) block_identifier);
        return NULL;
    }

    bonk_parser_eat_lexeme(parser);

    s_tree_node* block_definition = tree_create_typed_node(TREE_NODE_TYPE_BLOCK_DEFINITION);
    block_definition->left = (s_tree_node*) block_identifier;
    block_definition->right = block;

    return block_definition;
}


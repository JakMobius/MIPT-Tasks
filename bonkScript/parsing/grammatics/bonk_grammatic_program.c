/*
 * Program grammatic
 * $PROGRAM = $BLOCK_DEFINITION+
 */

#include "bonk_grammatic_program.h"

s_tree_node* bonk_parse_grammatic_program(s_bonk_parser* parser) {
    s_tree_node* list = tree_create_typed_node(TREE_NODE_TYPE_LIST);

    if(!list) {
        compiler_out_of_memory(parser->compiler);
        return NULL;
    }

    s_tree_node* result = list;

    s_tree_node* block = bonk_parse_grammatic_block_definition(parser);
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

        block = bonk_parse_grammatic_block_definition(parser);
    }

    return result;
}
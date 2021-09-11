/*
 * Block grammatic
 * $BLOCK := $EXPRESSION*
 */

#include "bonk_grammatic_block.h"

s_tree_node* bonk_parse_code_line(s_bonk_parser* parser) {
    s_tree_node* expression = bonk_parse_grammatic_expression_leveled(parser, true);
    if(expression) return expression;
    if(parser->compiler->state) return NULL;

    return NULL;
}

s_tree_node* bonk_parse_grammatic_block(s_bonk_parser* parser) {

    s_tree_node* list = tree_create_typed_node(TREE_NODE_TYPE_LIST);

    if(!list) {
        compiler_out_of_memory(parser->compiler);
        return NULL;
    }

    s_tree_node* result = list;

    s_tree_node* block = NULL;

    do {
        block = bonk_parse_code_line(parser);
        if(parser->compiler->state) return NULL;

        if(!block) break;

        list->left = block;
        list->right = tree_create_typed_node(TREE_NODE_TYPE_LIST);

        if(!list->right) {
            compiler_out_of_memory(parser->compiler);
            tree_release_subtree(list);
            return NULL;
        }
        list = list->right;
    } while(true);

    return result;
}
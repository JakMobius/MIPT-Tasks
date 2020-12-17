
#ifndef BONKSCRIPT_BONK_GRAMMATIC_SUB_BLOCK_H
#define BONKSCRIPT_BONK_GRAMMATIC_SUB_BLOCK_H

#include "../bonk_parser.h"

s_tree_node* bonk_parse_grammatic_sub_block(s_bonk_parser* parser);
s_tree_node* bonk_parse_grammatic_nested_block(s_bonk_parser* parser);
s_tree_node* bonk_parse_grammatic_cycle(s_bonk_parser* parser);
s_tree_node* bonk_parse_grammatic_check(s_bonk_parser* parser);

#endif //BONKSCRIPT_BONK_GRAMMATIC_SUB_BLOCK_H

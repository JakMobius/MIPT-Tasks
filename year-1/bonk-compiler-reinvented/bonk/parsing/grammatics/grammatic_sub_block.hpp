
#ifndef BONKSCRIPT_BONK_GRAMMATIC_SUB_BLOCK_H
#define BONKSCRIPT_BONK_GRAMMATIC_SUB_BLOCK_H

#include "../parser.hpp"

namespace bonk {

tree_node* parse_grammatic_sub_block(parser* parser);
tree_node_list<tree_node*>* parse_grammatic_nested_block(parser* parser);
tree_node_cycle* parse_grammatic_cycle(parser* parser);
tree_node_check* parse_grammatic_check(parser* parser);

}

#endif //BONKSCRIPT_BONK_GRAMMATIC_SUB_BLOCK_H

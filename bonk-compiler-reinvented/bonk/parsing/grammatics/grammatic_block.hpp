
#ifndef BONKSCRIPT_BONK_GRAMMATIC_BLOCK_H
#define BONKSCRIPT_BONK_GRAMMATIC_BLOCK_H

#include "../parser.hpp"
#include "grammatic_expression.hpp"

namespace bonk {

tree_node_list<tree_node*>* parse_grammatic_block(parser* parser);

}

#endif //BONKSCRIPT_BONK_GRAMMATIC_BLOCK_H


#ifndef BONKSCRIPT_BONK_GRAMMATIC_PROGRAM_H
#define BONKSCRIPT_BONK_GRAMMATIC_PROGRAM_H

#include "../parser.hpp"
#include "./grammatic_block_definition.hpp"

namespace bonk {

bool parse_grammatic_program(parser* parser, tree_node_list<tree_node*>* target);

}

#endif //BONKSCRIPT_BONK_GRAMMATIC_PROGRAM_H

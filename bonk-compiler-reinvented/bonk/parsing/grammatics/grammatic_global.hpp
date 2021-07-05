#ifndef BONKSCRIPT_BONK_GRAMMATIC_GLOBAL_H
#define BONKSCRIPT_BONK_GRAMMATIC_GLOBAL_H

#include "../parser.hpp"
#include "./grammatic_program.hpp"
#include "grammatic_help.hpp"

namespace bonk {

bool parse_grammatic_global(parser* parser, tree_node_list<tree_node*>* target);

}

#endif //BONKSCRIPT_BONK_GRAMMATIC_GLOBAL_H

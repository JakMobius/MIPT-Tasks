
#ifndef BONKSCRIPT_BONK_GRAMMATIC_REFERENCE_H
#define BONKSCRIPT_BONK_GRAMMATIC_REFERENCE_H

#include "../parser.hpp"
#include "./grammatic_expression.hpp"
#include <ctype.h>

namespace bonk {

tree_node* parse_grammatic_reference(parser* parser);
tree_node_list<tree_node_call_parameter*>* parse_grammatic_arguments(parser* parser);

}

#endif //BONKSCRIPT_BONK_GRAMMATIC_REFERENCE_H

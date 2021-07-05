

#ifndef BONKSCRIPT_BONK_GRAMMATIC_LOGIC_EXPRESSION_H
#define BONKSCRIPT_BONK_GRAMMATIC_LOGIC_EXPRESSION_H

#include "../parser.hpp"
#include "grammatic_logic_term.hpp"

namespace bonk {

tree_node* parse_grammatic_logic_expression(parser* parser);

}

#endif //BONKSCRIPT_BONK_GRAMMATIC_LOGIC_EXPRESSION_H

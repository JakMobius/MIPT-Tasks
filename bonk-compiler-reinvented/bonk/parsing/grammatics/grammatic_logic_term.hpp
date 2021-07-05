
#ifndef BONKSCRIPT_BONK_GRAMMATIC_LOGIC_TERM_H
#define BONKSCRIPT_BONK_GRAMMATIC_LOGIC_TERM_H

#include "../parser.hpp"
#include "grammatic_unary_operator.hpp"
#include "grammatic_assignment.hpp"
#include "grammatic_comparation.hpp"

namespace bonk {

tree_node* parse_grammatic_logic_term(parser* parser);

}

#endif //BONKSCRIPT_BONK_GRAMMATIC_LOGIC_TERM_H

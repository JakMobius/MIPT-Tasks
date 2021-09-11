
#ifndef BONKSCRIPT_BONK_GRAMMATIC_COMPARATION_H
#define BONKSCRIPT_BONK_GRAMMATIC_COMPARATION_H

#include "../parser.hpp"
#include "grammatic_math_expression.hpp"

namespace bonk {

bool is_comparation_operator(operator_type oper);
tree_node* parse_grammatic_comparation(parser* parser);

}

#endif //BONKSCRIPT_BONK_GRAMMATIC_COMPARATION_H

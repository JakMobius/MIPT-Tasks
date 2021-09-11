#ifndef BONKSCRIPT_BONK_GRAMMATIC_MATH_EXPRESSION_H
#define BONKSCRIPT_BONK_GRAMMATIC_MATH_EXPRESSION_H

#include "../parser.hpp"
#include "grammatic_math_term.hpp"

namespace bonk {

tree_node* parse_grammatic_math_expression(parser* parser);

}

#endif //BONKSCRIPT_BONK_GRAMMATIC_MATH_EXPRESSION_H

#ifndef BONKSCRIPT_BONK_GRAMMATIC_EXPRESSION_H
#define BONKSCRIPT_BONK_GRAMMATIC_EXPRESSION_H

#include "../parser.hpp"
#include "grammatic_logic_expression.hpp"
#include "grammatic_var_definition.hpp"
#include "grammatic_assignment.hpp"
#include "grammatic_unary_operator.hpp"
#include "grammatic_sub_block.hpp"

namespace bonk {

tree_node* parse_grammatic_expression(parser* parser);
tree_node* parse_grammatic_expression_leveled(parser* parser, bool top_level);

}

#endif //BONKSCRIPT_BONK_GRAMMATIC_EXPRESSION_H

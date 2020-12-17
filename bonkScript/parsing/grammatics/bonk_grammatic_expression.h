#ifndef BONKSCRIPT_BONK_GRAMMATIC_EXPRESSION_H
#define BONKSCRIPT_BONK_GRAMMATIC_EXPRESSION_H

#include "../bonk_parser.h"
#include "bonk_grammatic_logic_expression.h"
#include "bonk_grammatic_var_definition.h"
#include "bonk_grammatic_assignment.h"
#include "bonk_grammatic_unary_operator.h"
#include "bonk_grammatic_sub_block.h"

s_tree_node* bonk_parse_grammatic_expression(s_bonk_parser* parser);
s_tree_node* bonk_parse_grammatic_expression_leveled(s_bonk_parser* parser, bool top_level);

#endif //BONKSCRIPT_BONK_GRAMMATIC_EXPRESSION_H

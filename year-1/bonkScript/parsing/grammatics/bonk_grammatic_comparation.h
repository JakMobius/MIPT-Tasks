
#ifndef BONKSCRIPT_BONK_GRAMMATIC_COMPARATION_H
#define BONKSCRIPT_BONK_GRAMMATIC_COMPARATION_H

#include "../bonk_parser.h"
#include "bonk_grammatic_math_expression.h"

bool bonk_is_comparation_operator(e_bonk_operator_type operator);
s_tree_node* bonk_parse_grammatic_comparation(s_bonk_parser* parser);

#endif //BONKSCRIPT_BONK_GRAMMATIC_COMPARATION_H

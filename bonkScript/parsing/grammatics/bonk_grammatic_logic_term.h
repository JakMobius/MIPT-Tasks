
#ifndef BONKSCRIPT_BONK_GRAMMATIC_LOGIC_TERM_H
#define BONKSCRIPT_BONK_GRAMMATIC_LOGIC_TERM_H

#include "../bonk_parser.h"
#include "bonk_grammatic_unary_operator.h"
#include "bonk_grammatic_assignment.h"
#include "bonk_grammatic_comparation.h"

s_tree_node* bonk_parse_grammatic_logic_term(s_bonk_parser* parser);

#endif //BONKSCRIPT_BONK_GRAMMATIC_LOGIC_TERM_H

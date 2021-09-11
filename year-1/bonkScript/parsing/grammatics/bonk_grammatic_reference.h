
#ifndef BONKSCRIPT_BONK_GRAMMATIC_REFERENCE_H
#define BONKSCRIPT_BONK_GRAMMATIC_REFERENCE_H

#include "../bonk_parser.h"
#include "./bonk_grammatic_expression.h"
#include <ctype.h>

s_tree_node* bonk_parse_grammatic_reference(s_bonk_parser* parser);
s_tree_node* bonk_parse_grammatic_arguments(s_bonk_parser* parser);

#endif //BONKSCRIPT_BONK_GRAMMATIC_REFERENCE_H

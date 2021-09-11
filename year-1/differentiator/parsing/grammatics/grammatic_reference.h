
#ifndef BONKSCRIPT_GRAMMATIC_REFERENCE_H
#define BONKSCRIPT_GRAMMATIC_REFERENCE_H

#include "../parser.h"
#include <ctype.h>

s_tree_node* parse_grammatic_reference(s_parser* parser);
s_tree_node* parse_grammatic_varname(s_parser* parser);

#endif //BONKSCRIPT_GRAMMATIC_REFERENCE_H


#ifndef BONKSCRIPT_BONK_GRAMMATIC_HELP_H
#define BONKSCRIPT_BONK_GRAMMATIC_HELP_H

#include "../parser.hpp"
#include "errno.h"
#include "../../../utils/file_io.hpp"

namespace bonk {

bool parse_grammatic_help(parser* thou, tree_node_list<tree_node*>* target);

}

#endif //BONKSCRIPT_BONK_GRAMMATIC_HELP_H

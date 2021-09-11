

#ifndef AKINATOR_TREE_SEARCHER_H
#define AKINATOR_TREE_SEARCHER_H

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "tree.h"

typedef struct tree_searcher {
    s_tree_node** path;
    int path_buffer_length;
    bool is_running;
    tree_value_type target;
    bool found;
} s_tree_searcher;

const s_tree_node** tree_searcher_object_path(s_tree* tree, tree_value_type value);
bool tree_searcher_search_subtree(s_tree_node* node, s_tree_searcher* searcher);

#endif //AKINATOR_TREE_SEARCHER_H

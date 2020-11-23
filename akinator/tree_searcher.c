//
// Created by Темыч on 15.11.2020.
//

#include "tree_searcher.h"

const bool* tree_searcher_object_path(s_tree* tree, tree_value_type value) {
    if(!tree->root) {
        return NULL;
    }

    s_tree_searcher searcher = {0};
    searcher.path_buffer_length = 64;

    bool* buffer = malloc(searcher.path_buffer_length);

    searcher.path = &buffer;
    searcher.is_running = true;
    searcher.found = false;
    searcher.target = value;

    tree_searcher_search_subtree(tree->root, &searcher, 0);

    if(searcher.found) {
        return *searcher.path;
    } else {
        free((void*)*searcher.path);
        return NULL;
    }
}

void tree_searcher_search_subtree(s_tree_node* node, s_tree_searcher* searcher, int depth) {
    if(depth >= searcher->path_buffer_length) {
        searcher->path_buffer_length *= 2;
        bool* reallocated = realloc((void*)*searcher->path, searcher->path_buffer_length);
        if(!reallocated) {
            searcher->is_running = false;
        }
        *searcher->path = reallocated;
    }

    if(!searcher->is_running) return;

    if(!node->left) {
        if(strcmp(searcher->target, node->value) == 0) {
            searcher->found = true;
        }
        return;
    }

    (*searcher->path)[depth] = false;
    tree_searcher_search_subtree(node->left, searcher, depth + 1);

    if(searcher->found) return;
    if(!searcher->is_running) return;

    (*searcher->path)[depth] = true;
    tree_searcher_search_subtree(node->right, searcher, depth + 1);
}

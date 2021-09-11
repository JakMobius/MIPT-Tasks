
#include "tree_searcher.h"

const s_tree_node** tree_searcher_object_path(s_tree* tree, tree_value_type value) {
    if(!tree->root) {
        return NULL;
    }

    s_tree_searcher searcher = {0};
    searcher.path_buffer_length = 64;

    s_tree_node** buffer = calloc(searcher.path_buffer_length, sizeof(s_tree_node*));

    searcher.path = buffer;
    searcher.is_running = true;
    searcher.found = false;
    searcher.target = value;

    tree_searcher_search_subtree(tree->root, &searcher);

    if(searcher.found) {
        return (const s_tree_node**) searcher.path;
    } else {
        free((void*)searcher.path);
        return NULL;
    }
}

bool tree_searcher_search_subtree(s_tree_node* node, s_tree_searcher* searcher) {
    int depth = 0;
    searcher->path[0] = node;

    while(true) {
        s_tree_node* top = searcher->path[depth];

        if(top->left || top->right) {
            depth++;
            if(depth >= searcher->path_buffer_length) {
                searcher->path_buffer_length *= 2;
                s_tree_node** reallocated = realloc(searcher->path, searcher->path_buffer_length * sizeof(s_tree_node*));
                if(!reallocated) return false;
                searcher->path = reallocated;
            }

            if(top->left) {
                searcher->path[depth] = top->left;
            } else {
                searcher->path[depth] = top->right;
            }
        } else {
            if(strcmp(top->value, searcher->target) == 0) {
                searcher->found = true;
                return true;
            }

            if(depth == 0) return true;

            while(searcher->path[depth] == searcher->path[depth - 1]->right) {
                depth--;
                if(depth == 0) return true;
            }
            searcher->path[depth] = searcher->path[depth - 1]->right;
        }
    }
}

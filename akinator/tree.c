
#include "tree.h"

e_tree_error tree_resize(s_tree* tree, unsigned long capacity);

s_tree *tree_new(unsigned long capacity) {

    s_tree* tree = calloc(1, sizeof(s_tree));

    if(!tree) return NULL;

    tree_construct(tree, capacity);

    return tree;
}

e_tree_error tree_construct(s_tree *tree, unsigned long capacity) {

    tree->buffer_length = capacity;
    tree->nodes = tree_node_new(capacity);
    tree->unused_node_amount = capacity;
    tree->capacity = capacity;
    tree->root = NULL;

    if(!tree->nodes) return TREE_ERROR_OUT_OF_MEMORY;

    tree->unused_node_indices = calloc(capacity, sizeof(unsigned long));
    for(unsigned long i = 0; i < capacity; i++)
        tree->unused_node_indices[i] = i;

    return TREE_OK;
}

void tree_destruct(s_tree* tree) {
    tree_node_release(tree->root);
    free(tree->unused_node_indices);
    free(tree->nodes);
}

void tree_release(s_tree *tree) {
    if(!tree) return;
    tree_destruct(tree);
    free(tree);
}

s_tree_node *tree_node_new(unsigned long amount) {

    s_tree_node* tree = calloc(1, sizeof(s_tree_node) * amount);

    for(int i = 0; i < amount; i++) {
        tree_node_construct(tree + i);
    }

    return tree;
}

e_tree_error tree_node_resize(s_tree_node** buffer, unsigned long old_amount, unsigned long new_amount) {
    if(!buffer) return TREE_ERROR_NULL;
    s_tree_node* tree = realloc(*buffer, sizeof(s_tree_node) * new_amount);

    if(!tree) return TREE_ERROR_OUT_OF_MEMORY;

    for(unsigned long i = old_amount; i < new_amount; i++) {
        tree_node_construct(tree + i);
    }

    *buffer = tree;

    return TREE_OK;
}

e_tree_error tree_node_construct(s_tree_node* node) {
    if(!node) return TREE_ERROR_NULL;

    node->left = NULL;
    node->right = NULL;

    return TREE_OK;
}

void tree_node_release(s_tree_node *node) {
    if(!node) return;
    free((void*)node->value);
    tree_node_release(node->left);
    tree_node_release(node->right);
}

e_tree_error tree_get_free_node(s_tree* tree, s_tree_node** node) {
    if(!tree || !node) return TREE_ERROR_NULL;

    if(tree->unused_node_amount == 0) {
        e_tree_error error = tree_resize(tree, tree->capacity * 2);
        if(error) return error;
    }

    *node = tree->nodes + tree->unused_node_indices[--tree->unused_node_amount];
    (*node)->value = NULL;
    (*node)->left = NULL;
    (*node)->right = NULL;

    return TREE_OK;
}

e_tree_error tree_resize(s_tree* tree, unsigned long capacity) {
    unsigned long* reallocated = realloc(tree->unused_node_indices, capacity * sizeof(unsigned long));
    if(reallocated == NULL) return TREE_ERROR_OUT_OF_MEMORY;
    tree->unused_node_indices = reallocated;

    e_tree_error error = tree_node_resize(&tree->nodes, tree->capacity, capacity);
    if(error) {
        tree->unused_node_indices = realloc(tree->unused_node_indices, tree->capacity);
        return error;
    }

    for(unsigned long i = tree->capacity, unused_node_index = tree->unused_node_amount; i < capacity; i++) {
        tree->unused_node_indices[unused_node_index++] = i;
    }
    tree->unused_node_amount += capacity - tree->capacity;
    tree->capacity = capacity;

    return TREE_OK;
}
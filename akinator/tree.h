
#ifndef AKINATOR_TREE_H
#define AKINATOR_TREE_H

#include <stdlib.h>
#include <stdio.h>

typedef const char* tree_value_type;

typedef struct tree_node {
    struct tree_node* left;
    struct tree_node* right;
    tree_value_type value;
} s_tree_node;

typedef struct tree {
    s_tree_node* root;
    unsigned long buffer_length;
    unsigned long unused_node_amount;
    unsigned long* unused_node_indices;
    unsigned long capacity;
    s_tree_node* nodes;
} s_tree;

typedef enum tree_error {
    TREE_OK,
    TREE_NOT_FOUND,
    TREE_ERROR_NULL,
    TREE_ERROR_OUT_OF_MEMORY,
    TREE_ERROR_INVALID_FILE_FORMAT,
    TREE_ERROR_NOT_EMPTY,
} e_tree_error;

s_tree* tree_new(unsigned long capacity);
e_tree_error tree_construct(s_tree* tree, unsigned long capacity);
void tree_destruct(s_tree* tree);
void tree_release(s_tree* tree);

s_tree_node* tree_node_new(unsigned long amount);
e_tree_error tree_node_construct(s_tree_node* node);

e_tree_error tree_get_free_node(s_tree* tree, s_tree_node** node);

void tree_node_release(s_tree_node* node);

#endif //AKINATOR_TREE_H

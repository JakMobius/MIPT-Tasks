
#ifndef BONK_COMPILER_REINVENTED_AVL_TREE_HPP
#define BONK_COMPILER_REINVENTED_AVL_TREE_HPP

#include <cstdlib>
#include "default_alloc_construct.hpp"
#include "dynamic_array.hpp"

template <typename T>
struct avl_tree_node {
    struct avl_tree_node<T>* left;
    struct avl_tree_node<T>* right;
    int height;
    T value;
};

template <typename T>
struct avl_tree {
    typedef avl_tree_node<T> tree_node;
    tree_node* root;
    int size;
    
    static avl_tree* create();
    void construct();

    void destruct();
    void release();

    tree_node* node_create(T key);
    int node_balance_factor(tree_node* node);
    int node_height(avl_tree_node<T>* node);
    void node_fix_height(tree_node* node);
    tree_node* node_rotate_right(tree_node* node);
    tree_node* node_rotate_left(tree_node* node);
    tree_node* node_balance(tree_node* node);
    tree_node* node_insert(tree_node* node, T key);
    tree_node* node_find_min(tree_node* node);
    tree_node* node_remove_min(tree_node* node);
    tree_node* node_remove(tree_node* node, T key);
    bool node_exists(tree_node* node, T key);
    bool insert(T key);
    bool erase(T key);
    bool exists(T key);
    void free_node(tree_node* node);
    int depth(tree_node* node);
    tree_node* find_max();
    tree_node* find_min();

    void release_node(avl_tree_node<T>* node);

    dynamic_array<T>* values();
    void append_values(dynamic_array<T>* storage, avl_tree_node<T>* node);
};

bool avl_tree_node_less(int a, int b);

template <typename T>
avl_tree<T>* avl_tree<T>::create() {
    DEFAULT_ALLOC_CONSTRUCT(avl_tree<T>)
}

template <typename T>
void avl_tree<T>::construct() {
    size = 0;
    root = nullptr;
}

template <typename T>
void avl_tree<T>::release_node(avl_tree_node<T>* node) {
    free(node);
}

template <typename T>
int avl_tree<T>::node_height(avl_tree_node<T>* node) {
    if(node == NULL) return 0;
    return node->height;
}

template <typename T>
int avl_tree<T>::node_balance_factor(avl_tree_node<T>* node) {
    return node_height(node->right) - node_height(node->left);
}

template <typename T>
void avl_tree<T>::node_fix_height(avl_tree_node<T>* node) {
    int height_left = node_height(node->left);
    int height_right = node_height(node->right);

    if(height_left > height_right) node->height = height_left + 1;
    else node->height = height_right + 1;
}

template <typename T>
avl_tree_node<T>* avl_tree<T>::node_rotate_right(avl_tree_node<T>* node) {
    tree_node* left = node->left;
    node->left = left->right;
    left->right = node;

    node_fix_height(node);
    node_fix_height(left);

    return left;
}

template <typename T>
avl_tree_node<T>* avl_tree<T>::node_rotate_left(avl_tree_node<T>* node) {
    tree_node* right = node->right;
    node->right = right->left;
    right->left = node;

    node_fix_height(node);
    node_fix_height(right);

    return right;
}

template <typename T>
avl_tree_node<T>* avl_tree<T>::node_balance(avl_tree_node<T>* node) {
    node_fix_height(node);

    if(node_balance_factor(node) == -2)
    {
        if(node_balance_factor(node->left) > 0) {
            node->left = node_rotate_left(node->left);
        }
        return node_rotate_right(node);
    }

    if(node_balance_factor(node) == 2)
    {
        if(node_balance_factor(node->right) < 0) {
            node->right = node_rotate_right(node->right);
        }
        return node_rotate_left(node);
    }

    return node;
}

template <typename T>
avl_tree_node<T>* avl_tree<T>::node_insert(avl_tree_node<T>* node, T key) {
    if(!node) {
        auto result = node_create(key);
        if(result != nullptr) size++;
        return result;
    }

    if(key == node->value) return node;

    if(avl_tree_node_less(key, node->value)) {
        auto result = node_insert(node->left, key);
        if(!result) return nullptr;
        node->left = result;
    } else {
        auto result = node_insert(node->right, key);
        if(!result) return nullptr;
        node->right = result;
    }

    return node_balance(node);
}

template <typename T>
avl_tree_node<T>* avl_tree<T>::node_find_min(avl_tree_node<T>* node) {
    while(node->left) node = node->left;
    return node;
}

template <typename T>
avl_tree_node<T>* avl_tree<T>::node_remove_min(avl_tree_node<T>* node) {
    if(node->left == 0) return node->right;

    node->left = node_remove_min(node->left);

    return node_balance(node);
}

template <typename T>
avl_tree_node<T>* avl_tree<T>::node_remove(avl_tree_node<T>* node, T key) {
    if(!node) {
        return nullptr;
    } else if(avl_tree_node_less(key, node->value)) {
        node->left = node_remove(node->left, key);
    } else if(avl_tree_node_less(node->value, key)) {
        node->right = node_remove(node->right, key);
    } else {
        tree_node* left = node->left;
        tree_node* right = node->right;
        size--;
        release_node(node);

        if(!right) return left;

        tree_node* min = node_find_min(right);
        min->right = node_remove_min(right);
        min->left = left;
        return node_balance(min);
    }
    return node_balance(node);
}

template <typename T>
bool avl_tree<T>::node_exists(avl_tree_node<T>* node, T key) {
    if(!node) {
        return false;
    } else if(avl_tree_node_less(key, node->value)) {
        return node_exists(node->left, key);
    } else if(avl_tree_node_less(node->value, key)) {
        return node_exists(node->right, key);
    }
    return true;
}

template <typename T>
void avl_tree<T>::destruct() {
    free_node(root);
}

template <typename T>
void avl_tree<T>::release() {
    destruct();
    free(this);
}

template <typename T>
bool avl_tree<T>::insert(T key) {
    if(root == nullptr) {
        size++;
        root = node_create(key);
        return root != nullptr;
    } else {
        auto result = node_insert(root, key);
        if(result == nullptr) return false;
        root = result;
        return true;
    }
}

template <typename T>
bool avl_tree<T>::erase(T key) {
    int old_size = size;
    root = node_remove(root, key);
    return size != old_size;
}

template <typename T>
bool avl_tree<T>::exists(T key) {
    return node_exists(root, key);
}

template <typename T>
void avl_tree<T>::free_node(avl_tree_node<T>* node) {
    if(!node) return;
    free_node(node->left);
    free_node(node->right);
    free(node);
}

template <typename T>
int avl_tree<T>::depth(avl_tree_node<T>* node) {
    if(!node->left) return 1;
    return depth(node->left) + 1;
}

template <typename T>
avl_tree_node<T>* avl_tree<T>::find_max() {
    avl_tree_node<T>* min = root;
    if(!min) return nullptr;
    while(min->right) min = min->right;
    return min;
}

template <typename T>
avl_tree_node<T>* avl_tree<T>::find_min() {
    avl_tree_node<T>* min = root;
    if(!min) return nullptr;
    while(min->left) min = min->left;
    return min;
}

template<typename T>
avl_tree_node<T>* avl_tree<T>::node_create(T key) {

    tree_node* node = (tree_node*)calloc(1, sizeof(tree_node));

    if(node == NULL) return NULL;

    node->value = key;
    node->left = 0;
    node->right = 0;
    node->height = 1;

    return node;
}

template<typename T>
void avl_tree<T>::append_values(dynamic_array<T>* storage, avl_tree_node<T>* node) {
    if(!node) return;
    storage->push(node->value);
    append_values(storage, node->left);
    append_values(storage, node->right);
}

template<typename T>
dynamic_array<T>* avl_tree<T>::values() {
    auto result = dynamic_array<T>::create(size);

    append_values(result, root);

    return result;
}


#endif //BONK_COMPILER_REINVENTED_AVL_TREE_HPP

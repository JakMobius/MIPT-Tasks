
#ifndef AKINATOR_TREE_H
#define AKINATOR_TREE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

typedef enum tree_node_type {
    TREE_NODE_TYPE_NUMBER,
    TREE_NODE_TYPE_VARIABLE,
    TREE_NODE_TYPE_OPERATOR,
    TREE_NODE_TYPE_FUNCTION
} e_tree_node_type;

typedef enum tree_node_operator_type {
    TREE_OPERATOR_TYPE_PLUS,
    TREE_OPERATOR_TYPE_MINUS,
    TREE_OPERATOR_TYPE_MULTIPLY,
    TREE_OPERATOR_TYPE_DIVIDE,
    TREE_OPERATOR_TYPE_POW,
    TREE_OPERATOR_TYPE_DIFFERENTIAL,
    TREE_OPERATOR_TYPE_CALL
} e_tree_node_operator_type;

typedef enum tree_node_operator_flag {
    TREE_OPERATOR_FLAG_LEFT_HAND            = 0b0000000000000001,
    TREE_OPERATOR_FLAG_RIGHT_HAND           = 0b0000000000000010,
    TREE_OPERATOR_FLAG_LEFT_HAND_OPTIONAL   = 0b0000000000000100,
    TREE_OPERATOR_FLAG_RIGHT_HAND_OPTIONAL  = 0b0000000000001000,
    TREE_OPERATOR_FLAG_STRONG_GROUP         = 0b0000000000010000,
    TREE_OPERATOR_FLAG_PACIFIC_GROUP        = 0b0000000000100000,
    TREE_OPERATOR_FLAG_ASSOCIATIVE          = 0b0000000001000000,
} e_tree_node_operator_flag;

typedef enum tree_function {
    TREE_FUNCTION_SIN,
    TREE_FUNCTION_COS,
    TREE_FUNCTION_LN,
    TREE_FUNCTION_SQRT
} e_tree_function;

extern const int TREE_NODE_OPERATOR_PRIORITY[];
extern const char* TREE_NODE_OPERATOR_NAMES[];
extern const e_tree_node_operator_flag TREE_NODE_OPERATOR_FLAGS[];
extern const char* TREE_CONSTANT_NAMES[];
extern const char* TREE_FUNCTION_NAMES[];
extern const double* TREE_CONSTANT_VALUES[];
extern const int TREE_CONSTANT_AMOUNT;

typedef struct tree_node {
    struct tree_node* left;
    struct tree_node* right;
    e_tree_node_type type;
    struct tree_node* parent_reference;
} s_tree_node;

typedef struct tree_node_operator {
    s_tree_node tree_node;
    e_tree_node_operator_type operator_type;
} s_tree_node_operator;

typedef struct tree_node_number {
    s_tree_node tree_node;
    long long integer_value;
    long double float_value;
} s_tree_node_number;

typedef struct tree_node_variable {
    s_tree_node tree_node;
    const char* variable_name;
    unsigned long name_length;
    bool resolved;
    unsigned long variable_index;
} s_tree_node_variable;

typedef struct tree_node_function {
    s_tree_node tree_node;
    e_tree_function function;
} s_tree_node_function;

typedef struct tree {
    s_tree_node* root;
} s_tree;

typedef struct tree_evaluation_context {
    const double** variables;
} s_tree_evaluation_context;

typedef enum tree_error {
    TREE_OK,
    TREE_NOT_FOUND,
    TREE_ERROR_NULL,
    TREE_ERROR_OUT_OF_MEMORY,
    TREE_ERROR_INVALID_FILE_FORMAT,
    TREE_ERROR_NOT_EMPTY,
    TREE_ERROR_CANNOT_TAKE_DERIVATIVE,
    TREE_ERROR_CANNOT_EVALUATE
} e_tree_error;

#define TREE_NOT_FOUND wrap_error(TREE_NOT_FOUND);
#define TREE_ERROR_NULL wrap_error(TREE_ERROR_NULL);
#define TREE_ERROR_OUT_OF_MEMORY wrap_error(TREE_ERROR_OUT_OF_MEMORY);
#define TREE_ERROR_NOT_EMPTY wrap_error(TREE_ERROR_NOT_EMPTY);
#define TREE_ERROR_INVALID_FILE_FORMAT wrap_error(TREE_ERROR_INVALID_FILE_FORMAT);
#define TREE_ERROR_CANNOT_EVALUATE wrap_error(TREE_ERROR_CANNOT_EVALUATE)

e_tree_error wrap_error(e_tree_error error);
s_tree* tree_new();
e_tree_error tree_construct(s_tree* tree);
void tree_destruct(s_tree* tree);
void tree_release(s_tree* tree);

void tree_construct_node(s_tree_node* node);
s_tree_node* tree_create_node();

s_tree_node_function* tree_create_node_function(e_tree_function function_type);
void tree_construct_node_function(s_tree_node_function* function, e_tree_function function_type);

s_tree_node_number* tree_create_number_from_double(double value);
s_tree_node_number* tree_create_number_from_integer(long long value);

void tree_construct_node_operator(s_tree_node_operator* operator);
s_tree_node_operator* tree_create_node_operator(e_tree_node_operator_type type);

void tree_construct_node_number(s_tree_node_number* number);
s_tree_node_number* tree_create_node_number();

void tree_construct_node_variable(s_tree_node_variable* variable);
s_tree_node_variable* tree_create_node_variable();

e_tree_error tree_release_node(s_tree_node* node);
e_tree_error tree_release_subtree(s_tree_node* node);

s_tree_node* tree_node_copy(s_tree_node* node);
unsigned long tree_node_size(s_tree_node* node);
s_tree_node* tree_node_clone_deep(s_tree_node* node);

e_tree_error tree_evaluate(s_tree* tree, const double** variables, double* result);
e_tree_error tree_evaluate_node(s_tree_node* node, s_tree_evaluation_context* context, double* result);

void tree_node_sift_left(s_tree_node** node);
void tree_node_sift_right(s_tree_node** node);
s_tree_node* tree_left_leap(s_tree_node* node);
s_tree_node* tree_right_leap(s_tree_node* node);
bool tree_node_is_operator(s_tree_node* node, e_tree_node_operator_type operator);
e_tree_error tree_node_turn_to_number(s_tree_node* node, double number);
bool tree_node_is_exact_number(s_tree_node* node, double value);

void tree_validate(s_tree* tree);

void tree_node_release(s_tree_node* node);

#endif //AKINATOR_TREE_H

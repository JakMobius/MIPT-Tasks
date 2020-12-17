
#ifndef AKINATOR_TREE_H
#define AKINATOR_TREE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

typedef enum tree_node_type {
    TREE_NODE_TYPE_NUMBER,
    TREE_NODE_TYPE_IDENTIFIER,
    TREE_NODE_TYPE_OPERATOR,
    TREE_NODE_TYPE_LIST,
    TREE_NODE_TYPE_BLOCK_DEFINITION,
    TREE_NODE_TYPE_VAR_DEFINITION,
    TREE_NODE_TYPE_ASSIGNMENT,
    TREE_NODE_TYPE_ARGUMENT
} e_tree_node_type;

typedef struct tree s_tree;
typedef struct tree_node s_tree_node;
typedef struct tree_node_operator s_tree_node_operator;
typedef struct tree_node_number s_tree_node_number;
typedef struct tree_node_identifier s_tree_node_identifier;

#include "parsing/lexic/bonk_lexical_analyzer.h"

typedef struct tree {
    s_tree_node* root;
} s_tree;

typedef struct tree_node {
    struct tree_node* left;
    struct tree_node* right;
    e_tree_node_type type;
    struct tree_node* parent_reference;
    s_bonk_parser_position* source_position;
} s_tree_node;

typedef struct tree_node_operator {
    s_tree_node tree_node;
    e_bonk_operator_type operator_type;
} s_tree_node_operator;

typedef struct tree_node_number {
    s_tree_node tree_node;
    long long integer_value;
    long double float_value;
} s_tree_node_number;

typedef struct tree_node_identifier {
    s_tree_node tree_node;
    const char* variable_name;
    unsigned long name_length;
    unsigned long variable_index;
} s_tree_node_identifier;

typedef struct tree_node_variable_definition {
    s_tree_node tree_node;
    bool is_contextual;
} s_tree_node_variable_definition;

typedef enum tree_error {
    TREE_OK,
    TREE_NOT_FOUND,
    TREE_ERROR_NULL,
    TREE_ERROR_OUT_OF_MEMORY,
    TREE_ERROR_NOT_EMPTY,
} e_tree_error;

#define TREE_NOT_FOUND wrap_error(TREE_NOT_FOUND);
//#define TREE_ERROR_NULL wrap_error(TREE_ERROR_NULL);
#define TREE_ERROR_OUT_OF_MEMORY wrap_error(TREE_ERROR_OUT_OF_MEMORY);
#define TREE_ERROR_NOT_EMPTY wrap_error(TREE_ERROR_NOT_EMPTY);
//#define TREE_ERROR_INVALID_FILE_FORMAT wrap_error(TREE_ERROR_INVALID_FILE_FORMAT);

e_tree_error wrap_error(e_tree_error error);
s_tree* tree_new();
e_tree_error tree_construct(s_tree* tree);
void tree_destruct(s_tree* tree);
void tree_release(s_tree* tree);

s_tree_node_number* tree_create_number_from_double(double value);
s_tree_node_number* tree_create_number_from_integer(long long value);

void tree_construct_node(s_tree_node* node);
s_tree_node* tree_create_node();

s_tree_node* tree_create_typed_node(e_tree_node_type type);

void tree_construct_node_operator(s_tree_node_operator* operator);
s_tree_node_operator* tree_create_node_operator(e_bonk_operator_type type);

void tree_construct_node_number(s_tree_node_number* number);
s_tree_node_number* tree_create_node_number();

void tree_construct_node_identifier(s_tree_node_identifier* variable);
s_tree_node_identifier* tree_create_node_identifier();

void tree_construct_node_variable_definition(s_tree_node_variable_definition* variable_definition);
s_tree_node_variable_definition* tree_create_node_variable_definition();

bool tree_node_identifier_equals(s_tree_node_identifier* left, s_tree_node_identifier* right);
void bonk_print_identifier(FILE* file, s_tree_node_identifier* identifier);
const char* tree_node_identifier_name(s_tree_node_identifier* identifier);

e_tree_error tree_release_node(s_tree_node* node);
e_tree_error tree_release_subtree(s_tree_node* node);

s_tree_node* tree_node_clone(s_tree_node* node);

void tree_validate(s_tree* tree);

void tree_node_release(s_tree_node* node);

e_tree_error tree_serialize_subtree(s_tree_node* target, FILE* file);
e_tree_error tree_serialize(s_tree* target, FILE* file);

#endif //AKINATOR_TREE_H

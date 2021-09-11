#ifndef BONKSCRIPT_BONK_VARIABLE_H
#define BONKSCRIPT_BONK_VARIABLE_H

typedef struct bonk_variable s_bonk_variable;
typedef struct bonk_variable_function s_bonk_variable_function;

typedef enum bonk_variable_type {
    BONK_VARIABLE_TYPE_NUMBER,
    BONK_VARIABLE_TYPE_FUNCTION
} e_bonk_variable_type;

#include "tree.h"

typedef struct bonk_variable {
    e_bonk_variable_type type;
    s_tree_node_identifier* identifier;
    unsigned long byte_offset;
} s_bonk_variable;

typedef struct bonk_variable_function {
    s_bonk_variable super_structure;
    s_bonk_field_list* argument_list;
} s_bonk_variable_number;

void bonk_variable_construct(s_bonk_variable*, s_tree_node_identifier* identifier);

s_bonk_variable* bonk_variable_number_create(s_tree_node_identifier* identifier);
void bonk_variable_number_construct(s_bonk_variable* variable);

s_bonk_variable_function* bonk_variable_function_create(s_tree_node_identifier* identifier, s_bonk_field_list* argument_list);
void bonk_variable_function_construct(s_bonk_variable_function* function, s_bonk_field_list* argument_list);

void bonk_variable_destroy(s_bonk_variable* variable);
void bonk_variable_release(s_bonk_variable* variable);

#endif //BONKSCRIPT_BONK_VARIABLE_H
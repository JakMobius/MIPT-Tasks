
#ifndef BONKSCRIPT_BONK_FIELD_LIST_H
#define BONKSCRIPT_BONK_FIELD_LIST_H

typedef struct bonk_field_list s_bonk_field_list;

#include "./tree.h"

typedef struct bonk_field_list {
    unsigned long frame_size;
    unsigned long variables_amount;
    unsigned long variables_buffer_capacity;
    unsigned long byte_offset;
    s_bonk_variable** variables;
} s_bonk_field_list;

s_bonk_field_list* bonk_field_list_create();
void bonk_field_list_construct(s_bonk_field_list* scope);
s_bonk_variable* bonk_field_list_get_variable(s_bonk_field_list* scope, s_tree_node_identifier* identifier);
bool bonk_field_list_resize(s_bonk_field_list* scope, unsigned long new_capacity);
bool bonk_field_list_add_variable(s_bonk_field_list* scope, s_bonk_variable* variable);

void bonk_field_list_destroy(s_bonk_field_list* scope);
void bonk_field_list_release(s_bonk_field_list* scope);

#endif //BONKSCRIPT_BONK_FIELD_LIST_H

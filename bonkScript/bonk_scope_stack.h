
#ifndef BONKSCRIPT_BONK_SCOPE_STACK_H
#define BONKSCRIPT_BONK_SCOPE_STACK_H

typedef struct bonk_scope_stack s_bonk_scope_stack;

#include "./bonk_compiler.h"

typedef struct bonk_scope_stack {
    s_bonk_field_list** scopes;
    unsigned long scope_amount;
    unsigned long scope_buffer_capacity;
} s_bonk_stope_stack;

s_bonk_scope_stack* bonk_scope_stack_create();
void bonk_scope_stack_construct(s_bonk_scope_stack* stack);
void bonk_scope_stack_destruct();
void bonk_scope_stack_release();

bool bonk_scope_stack_push_scope(s_bonk_scope_stack* stack, s_bonk_field_list* scope);
void bonk_scope_stack_pop_scope(s_bonk_scope_stack* stack);
s_bonk_field_list* bonk_scope_stack_top(s_bonk_scope_stack* stack);

unsigned long bonk_scope_stack_frame_size(s_bonk_scope_stack* stack);
s_bonk_variable* bonk_scope_stack_get_variable(s_bonk_scope_stack* stack, s_tree_node_identifier* identifier, s_bonk_field_list** scope);

#endif //BONKSCRIPT_BONK_SCOPE_STACK_H
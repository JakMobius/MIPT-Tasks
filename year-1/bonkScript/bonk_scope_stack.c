
#include "bonk_scope_stack.h"

s_bonk_scope_stack* bonk_scope_stack_create() {
    s_bonk_scope_stack* new_stack = calloc(1, sizeof(s_bonk_scope_stack));
    if(!new_stack) return NULL;
    bonk_scope_stack_construct(new_stack);
    return new_stack;
}
void bonk_scope_stack_construct(s_bonk_scope_stack* stack) {
    stack->scope_amount = 0;
    stack->scope_buffer_capacity = 0;
    stack->scopes = NULL;
}

void bonk_scope_stack_destruct(s_bonk_scope_stack* stack) {
    free(stack->scopes);
}

void bonk_scope_stack_release(s_bonk_scope_stack* stack) {
    bonk_scope_stack_destruct(stack);
    free(stack);
}

bool bonk_scope_stack_resize(s_bonk_scope_stack* stack, unsigned long new_capacity) {
    s_bonk_field_list** new_buffer = realloc(stack->scopes, new_capacity * sizeof(s_bonk_field_list*));
    if(!new_buffer) return false;
    stack->scopes = new_buffer;

    return true;
}

bool bonk_scope_stack_push_scope(s_bonk_scope_stack* stack, s_bonk_field_list* scope) {
    if(stack->scope_amount == stack->scope_buffer_capacity) {
        unsigned long new_capacity = stack->scope_buffer_capacity * 2;
        if(new_capacity < 16) new_capacity = 16;
        if(!bonk_scope_stack_resize(stack, new_capacity)) return false;
        stack->scope_buffer_capacity = new_capacity;
    }

    if(stack->scope_amount > 0) {
        s_bonk_field_list* previous_scope = stack->scopes[stack->scope_amount - 1];

        scope->byte_offset = previous_scope->byte_offset + previous_scope->frame_size;
    }
    stack->scopes[stack->scope_amount++] = scope;
    return true;
}

void bonk_scope_stack_pop_scope(s_bonk_scope_stack* stack) {
    assert(stack->scope_amount);

    stack->scope_amount--;
}

s_bonk_field_list* bonk_scope_stack_top(s_bonk_scope_stack* stack) {
    assert(stack->scope_amount > 0);
    return stack->scopes[stack->scope_amount - 1];
}

unsigned long bonk_scope_stack_frame_size(s_bonk_scope_stack* stack) {
    unsigned long scope_offset = 0;
    for(unsigned long i = 0; i < stack->scope_amount; i++) {
        scope_offset += stack->scopes[i]->frame_size;
    }

    return scope_offset;
}

s_bonk_variable* bonk_scope_stack_get_variable(s_bonk_scope_stack* stack, s_tree_node_identifier* identifier, s_bonk_field_list** scope) {

    for(unsigned long i = stack->scope_amount - 1; i != (unsigned long) -1; i--) {

        s_bonk_variable* variable = bonk_field_list_get_variable(stack->scopes[i], identifier);

        if(variable != NULL) {
            if(scope) *scope = stack->scopes[i];
            return variable;
        }
    }

    return NULL;
}


#include "bonk_field_list.h"

s_bonk_field_list* bonk_field_list_create() {
    s_bonk_field_list* new_scope = calloc(1, sizeof(s_bonk_field_list));
    if(!new_scope) return NULL;
    bonk_field_list_construct(new_scope);
    return new_scope;
}

void bonk_field_list_construct(s_bonk_field_list* scope) {
    scope->variables_amount = 0;
    scope->variables_buffer_capacity = 0;
    scope->variables = NULL;
    scope->frame_size = 0;
    scope->byte_offset = 0;
}

s_bonk_variable* bonk_field_list_get_variable(s_bonk_field_list* scope, s_tree_node_identifier* identifier) {

    for(unsigned long i = 0; i < scope->variables_amount; i++) {
        if(tree_node_identifier_equals(scope->variables[i]->identifier, identifier)) {
            return scope->variables[i];
        }
    }

    return NULL;
}

bool bonk_field_list_resize(s_bonk_field_list* scope, unsigned long new_capacity) {
    s_bonk_variable** new_buffer = realloc(scope->variables, new_capacity * sizeof(s_bonk_variable*));
    if(!new_buffer) return false;
    scope->variables = new_buffer;
    return true;
}

bool bonk_field_list_add_variable(s_bonk_field_list* scope, s_bonk_variable* variable) {
    if(scope->variables_amount == scope->variables_buffer_capacity) {
        unsigned long new_capacity = scope->variables_buffer_capacity * 2;
        if(new_capacity < 16) new_capacity = 16;
        if(!bonk_field_list_resize(scope, new_capacity)) {
            return false;
        }
    }

    variable->byte_offset = scope->frame_size + scope->byte_offset;

    scope->frame_size += 8;
    scope->variables[scope->variables_amount++] = variable;

    return true;
}

void bonk_field_list_destroy(s_bonk_field_list* scope) {
    for(int i = 0; i < scope->variables_amount; i++) {
        bonk_variable_release(scope->variables[i]);
    }
    free(scope->variables);
}

void bonk_field_list_release(s_bonk_field_list* scope) {
    bonk_field_list_destroy(scope);
    free(scope);
}

#include "ede_scope_stack.hpp"

namespace bonk::ede_backend {

scope_stack* scope_stack::create() {
    CHECK_ALLOC_CONSTRUCT(scope_stack)
}

bool scope_stack::construct() {
    if(scopes.construct(4)) return false;
    return true;
}

void scope_stack::destruct() {
    scopes.destruct();
}

void scope_stack::release() {
    destruct();
    free(this);
}

bool scope_stack::push_scope(field_list* scope) {

    if(scopes.size > 0) {
        field_list* previous_scope = scopes.get(scopes.size - 1);

        scope->byte_offset = previous_scope->byte_offset + previous_scope->frame_size;
    }

    scopes.push(scope);
    return true;
}

void scope_stack::pop_scope() {
    scopes.pop();
}

field_list* scope_stack::top() {
    return scopes.get(scopes.size - 1);
}

unsigned long scope_stack::frame_size() {
    unsigned long scope_offset = 0;
    for(int i = 0; i < scopes.size; i++) {
        scope_offset += scopes.get(i)->frame_size;
    }

    return scope_offset;
}

variable* scope_stack::get_variable(tree_node_identifier* identifier, field_list** scope) {

    for(int i = scopes.size - 1; i >= 0; i--) {

        variable* var = scopes.get(i)->get_variable(identifier);

        if(var != nullptr) {
            if(scope) *scope = scopes.get(i);
            return var;
        }
    }

    return nullptr;
}

}
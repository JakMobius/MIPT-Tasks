
#include "x86_scope_stack.hpp"

namespace bonk::x86_backend {

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
    scopes.push(scope);
    return true;
}

void scope_stack::pop_scope() {
    scopes.pop();
}

field_list* scope_stack::top() {
    return scopes.get(scopes.size - 1);
}

variable* scope_stack::get_variable(tree_node_identifier* identifier, field_list** scope) {

    for(long long i = scopes.size - 1; i >= 0; i--) {

        variable* var = scopes.get(i)->get_variable(identifier);

        if(var != nullptr) {
            if(scope) *scope = scopes.get(i);
            return var;
        }
    }

    return nullptr;
}

}
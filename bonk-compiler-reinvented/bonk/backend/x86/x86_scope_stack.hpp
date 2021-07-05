
#ifndef BONKSCRIPT_BONK_SCOPE_STACK_H
#define BONKSCRIPT_BONK_SCOPE_STACK_H

namespace bonk::x86_backend {

struct scope_stack;

}

#include "../../compiler.hpp"
#include "x86_variable.hpp"
#include "../../../utils/dynamic_array.hpp"

namespace bonk::x86_backend {

struct scope_stack {
    dynamic_array<field_list*> scopes;

    static scope_stack* create();
    bool construct();
    void destruct();
    void release();

    bool push_scope(field_list* scope);
    void pop_scope();
    field_list* top();

    variable* get_variable(tree_node_identifier* identifier, field_list** scope);
};

}

#endif //BONKSCRIPT_BONK_SCOPE_STACK_H
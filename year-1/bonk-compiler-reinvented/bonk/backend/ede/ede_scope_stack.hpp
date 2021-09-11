
#ifndef BONK_COMPILER_REINVENTED_EDE_SCOPE_STACK_H
#define BONK_COMPILER_REINVENTED_EDE_SCOPE_STACK_H

namespace bonk::ede_backend {

struct scope_stack;

}

#include "../../compiler.hpp"
#include "ede_variable.hpp"
#include "../../../utils/dynamic_array.hpp"

namespace bonk::ede_backend {

struct scope_stack {
    dynamic_array<field_list*> scopes;

    static scope_stack* create();
    bool construct();
    void destruct();
    void release();

    bool push_scope(field_list* scope);
    void pop_scope();
    field_list* top();

    unsigned long frame_size();
    variable* get_variable(tree_node_identifier* identifier, field_list** scope);
};

}

#endif //BONK_COMPILER_REINVENTED_EDE_SCOPE_STACK_H
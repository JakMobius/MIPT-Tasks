
#ifndef BONKSCRIPT_BONK_FIELD_LIST_H
#define BONKSCRIPT_BONK_FIELD_LIST_H

namespace bonk::x86_backend {

struct field_list;

}

#include "x86_variable.hpp"
#include "x86_backend_context.hpp"
#include "../../../utils/dynamic_array.cpp"
#include "../../tree/nodes/nodes.hpp"

namespace bonk::x86_backend {

struct field_list {
    int arguments;
    dynamic_array<variable*> variables;
    backend_context_state* context_state;
    register_descriptor_list* descriptors;

    static field_list* create(register_descriptor_list* descriptor_list, backend_context_state* state);
    void construct(register_descriptor_list* descriptor_list, backend_context_state* state);
    variable* get_variable(tree_node_identifier* identifier);
    bool add_variable(variable* variable);

    void destroy();
    void release();
};

}
#endif //BONKSCRIPT_BONK_FIELD_LIST_H


#ifndef BONK_COMPILER_REINVENTED_EDE_FIELD_LIST_H
#define BONK_COMPILER_REINVENTED_EDE_FIELD_LIST_H

namespace bonk::ede_backend {

struct field_list;

}

#include "ede_variable.hpp"
#include "../../../utils/dynamic_array.cpp"

namespace bonk::ede_backend {

struct field_list {
    dynamic_array<variable*> variables;
    unsigned long byte_offset;
    unsigned long frame_size;

    static field_list* create();
    void construct();
    variable* get_variable(tree_node_identifier* identifier);
    bool add_variable(variable* variable);

    void destroy();
    void release();
};

}
#endif //BONK_COMPILER_REINVENTED_EDE_FIELD_LIST_H

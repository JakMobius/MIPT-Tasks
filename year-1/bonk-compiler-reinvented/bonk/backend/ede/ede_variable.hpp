#ifndef BONK_COMPILER_REINVENTED_EDE_VARIABLE_H
#define BONK_COMPILER_REINVENTED_EDE_VARIABLE_H

namespace bonk::ede_backend {

struct variable;
struct variable_function;

enum variable_type {
    VARIABLE_TYPE_NUMBER,
    VARIABLE_TYPE_FUNCTION
};

}

#include "../../tree/nodes/identifier.hpp"
#include "ede_field_list.hpp"

namespace bonk::ede_backend {

struct variable {
    variable_type type;
    tree_node_identifier* identifier;
    unsigned long byte_offset;

    void construct(tree_node_identifier* identifier);

    void destroy();
    void release();
};

struct variable_number : variable {
    static variable_number* create(tree_node_identifier* the_identifier);
    void construct(tree_node_identifier* the_identifier);
};

struct variable_function : variable {
    field_list* argument_list;

    static variable_function* create(tree_node_identifier* the_identifier, field_list* the_argument_list);
    void construct(tree_node_identifier* the_identifier, field_list* the_argument_list);
};



}

#endif //BONK_COMPILER_REINVENTED_EDE_VARIABLE_H
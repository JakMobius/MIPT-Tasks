//
// Created by Темыч on 23.04.2021.
//

#ifndef BONK_COMPILER_REINVENTED_X86_VARIABLE_HPP
#define BONK_COMPILER_REINVENTED_X86_VARIABLE_HPP

namespace bonk::x86_backend {

struct variable;
struct variable_function;

enum variable_type {
    VARIABLE_TYPE_NUMBER,
    VARIABLE_TYPE_FUNCTION
};

}

#include "../../tree/nodes/identifier.hpp"
#include "x86_field_list.hpp"
#include "x86_abstract_register.hpp"

namespace bonk::x86_backend {

struct variable {
    bool is_contextual;
    variable_type type;
    tree_node_identifier* identifier;
    abstract_register storage;

    void construct(tree_node_identifier* identifier);

    void destroy();
    void release();
};

struct variable_number : variable {
    static variable_number* create(tree_node_variable_definition* definition);
    void construct(tree_node_variable_definition* the_identifier);
};

struct variable_function : variable {
    field_list* argument_list;

    static variable_function* create(tree_node_identifier* the_identifier, field_list* the_argument_list);
    void construct(tree_node_identifier* the_identifier, field_list* the_argument_list);
};

}

#endif //BONK_COMPILER_REINVENTED_X86_VARIABLE_HPP

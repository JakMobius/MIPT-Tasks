//
// Created by Темыч on 20.04.2021.
//

#ifndef BONK_COMPILER_REINVENTED_VAR_DEFINITION_HPP
#define BONK_COMPILER_REINVENTED_VAR_DEFINITION_HPP

namespace bonk {

struct tree_node_variable_definition;

}

#include "node.hpp"
#include "identifier.hpp"
#include "../../../utils/inherit.cpp"
#include "../../../utils/json_serializer.hpp"

namespace bonk {

struct tree_node_variable_definition : tree_node {
    bool is_contextual;
    tree_node_identifier* variable_name;
    tree_node* variable_value;

    static tree_node_variable_definition* create(bool contextual, tree_node_identifier* identifier);
    void construct(bool contextual, tree_node_identifier* identifier);

    void virtual_serialize(json_serializer* file);
    void virtual_destroy();

    static const tree_node_methods methods;
};

}

#endif //BONK_COMPILER_REINVENTED_VAR_DEFINITION_HPP

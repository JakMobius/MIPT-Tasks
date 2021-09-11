//
// Created by Темыч on 20.04.2021.
//

#ifndef BONK_COMPILER_REINVENTED_CALL_PARAMETER_HPP
#define BONK_COMPILER_REINVENTED_CALL_PARAMETER_HPP

namespace bonk {

struct tree_node_call_parameter;

}


#include "node.hpp"
#include "identifier.hpp"
#include "../../../utils/json_serializer.hpp"

namespace bonk {

struct tree_node_call_parameter : tree_node {

    tree_node_identifier* parameter_name;
    tree_node* parameter_value;

    static tree_node_call_parameter* create(tree_node_identifier* name, tree_node* value);
    void construct(tree_node_identifier* name, tree_node* value);

    /* Override methods */
    void virtual_destruct();
    void virtual_serialize(json_serializer* serializer);

    static const tree_node_methods methods;
};

}

#endif //BONK_COMPILER_REINVENTED_CALL_PARAMETER_HPP

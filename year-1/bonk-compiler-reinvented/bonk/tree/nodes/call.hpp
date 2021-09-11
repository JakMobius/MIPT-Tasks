//
// Created by Темыч on 21.04.2021.
//

#ifndef BONK_COMPILER_REINVENTED_CALL_HPP
#define BONK_COMPILER_REINVENTED_CALL_HPP


#include "node.hpp"
#include "list.cpp"
#include "identifier.hpp"
#include "../../../utils/json_serializer.hpp"
#include "../../../utils/inherit.cpp"
#include "call_parameter.hpp"

namespace bonk {

struct tree_node_call : tree_node {

    tree_node_identifier* call_function;
    tree_node_list<tree_node_call_parameter*>* call_parameters;

    static tree_node_call* create(tree_node_identifier* function, tree_node_list<tree_node_call_parameter*>* parameters);
    void construct(tree_node_identifier* function, tree_node_list<tree_node_call_parameter*>* parameters);

    /* Override methods */
    void virtual_destruct();
    void virtual_serialize(json_serializer* serializer);

    static const tree_node_methods methods;
};

}

#endif //BONK_COMPILER_REINVENTED_CALL_HPP

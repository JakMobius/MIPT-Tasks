//
// Created by Темыч on 20.04.2021.
//

#ifndef BONK_COMPILER_REINVENTED_BLOCK_DEFINITION_HPP
#define BONK_COMPILER_REINVENTED_BLOCK_DEFINITION_HPP

namespace bonk {

struct tree_node_check;

}

#include "node.hpp"
#include "../../../utils/list.hpp"
#include "../../../utils/inherit.cpp"
#include "../../../utils/json_serializer.hpp"
#include "identifier.hpp"
#include "call_parameter.hpp"
#include "list.cpp"

namespace bonk {

struct tree_node_block_definition : tree_node {

    tree_node_identifier* block_name;
    tree_node_list<tree_node*>* body;
    bool is_promise;

    static tree_node_block_definition* create();
    void construct();

    /* Override methods */
    void virtual_destruct();

    static const tree_node_methods methods;

    void virtual_serialize(json_serializer* file);
};

}

#endif //BONK_COMPILER_REINVENTED_BLOCK_DEFINITION_HPP

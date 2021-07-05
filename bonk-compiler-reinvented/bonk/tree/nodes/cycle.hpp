//
// Created by Темыч on 20.04.2021.
//

#ifndef BONK_COMPILER_REINVENTED_CYCLE_HPP
#define BONK_COMPILER_REINVENTED_CYCLE_HPP

namespace bonk {

struct tree_node_cycle;

}


#include "node.hpp"
#include "list.cpp"
#include "../../../utils/json_serializer.hpp"

namespace bonk {

struct tree_node_cycle: tree_node {

    tree_node_list<tree_node*>* body;

    static tree_node_cycle* create();
    void construct();

    /* Override methods */
    void virtual_destruct();
    void virtual_serialize(json_serializer* serializer);

    static const tree_node_methods methods;
};

}

#endif //BONK_COMPILER_REINVENTED_CYCLE_HPP

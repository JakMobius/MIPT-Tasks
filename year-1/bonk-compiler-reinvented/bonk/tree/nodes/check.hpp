//
// Created by Темыч on 20.04.2021.
//

#ifndef BONK_COMPILER_REINVENTED_CHECK_HPP
#define BONK_COMPILER_REINVENTED_CHECK_HPP

namespace bonk {

struct tree_node_check;

}


#include "node.hpp"
#include "../../../utils/inherit.cpp"
#include "../../../utils/json_serializer.hpp"
#include "list.cpp"

namespace bonk {

struct tree_node_check : tree_node {

    tree_node* condition;
    tree_node_list<tree_node*>* check_body;
    tree_node_list<tree_node*>* or_body;

    static tree_node_check* create();
    void construct();

    /* Override methods */
    void virtual_destruct();
    void virtual_serialize(json_serializer* serializer);

    static const tree_node_methods methods;
};

}

#endif //BONK_COMPILER_REINVENTED_CHECK_HPP

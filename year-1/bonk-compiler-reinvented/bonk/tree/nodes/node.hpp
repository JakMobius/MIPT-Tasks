//
// Created by Темыч on 20.04.2021.
//

#ifndef BONK_COMPILER_REINVENTED_NODE_HPP
#define BONK_COMPILER_REINVENTED_NODE_HPP

#include "../../../utils/default_alloc_construct.hpp"
#include "../../parsing/parser_position.hpp"
#include "../../../utils/json_serializer.hpp"
#include "../../../utils/json_serializer.hpp"
#include <cstdio>

namespace bonk {

struct tree_node;
struct tree_node_methods;

}

namespace bonk {

enum tree_node_type {
    TREE_NODE_TYPE_NUMBER,
    TREE_NODE_TYPE_IDENTIFIER,
    TREE_NODE_TYPE_OPERATOR,
    TREE_NODE_TYPE_LIST,
    TREE_NODE_TYPE_BLOCK_DEFINITION,
    TREE_NODE_TYPE_VAR_DEFINITION,
    TREE_NODE_TYPE_CALL,
    TREE_NODE_TYPE_CALL_PARAMETER,
    TREE_NODE_TYPE_CHECK,
    TREE_NODE_TYPE_CYCLE
};

struct tree_node_methods {
    void (tree_node::*destruct)();
    void (tree_node::*serialize)(json_serializer*);
};

struct tree_node {
    tree_node_type type;
    parser_position* source_position;

    static tree_node* create();
    void construct();

    void destruct();
    void serialize(json_serializer* file);
    void release();

    /* Virtual methods */

    void virtual_destruct();
    void virtual_serialize(json_serializer* file);

    /* Inheritance stuff */
    const tree_node_methods* vtable;
    static const tree_node_methods methods;
};

}

#endif //BONK_COMPILER_REINVENTED_NODE_HPP

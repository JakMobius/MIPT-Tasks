//
// Created by Темыч on 20.04.2021.
//

#include "cycle.hpp"

namespace bonk {

tree_node_cycle* tree_node_cycle::create() {
    DEFAULT_ALLOC_CONSTRUCT(tree_node_cycle)
}

void tree_node_cycle::construct() {
    ((tree_node*) this)->construct();

    type = TREE_NODE_TYPE_CYCLE;
    vtable = &methods;
}

void tree_node_cycle::virtual_destruct() {

}

void tree_node_cycle::virtual_serialize(json_serializer* serializer) {
    (*this.*tree_node::methods.serialize)(serializer);

    serializer->block_string_field("type", "cycle");
    if(body) {
        serializer->block_start_block("body");
        body->serialize(serializer);
        serializer->close_block();
    } else {
        serializer->block_string_field("body", nullptr);
    }
}

const tree_node_methods tree_node_cycle::methods = inherit(tree_node::methods, tree_node_methods {
    .destruct = (void (tree_node::*)()) &tree_node_cycle::virtual_destruct,
    .serialize = (void (tree_node::*)(json_serializer*)) &tree_node_cycle::virtual_serialize
});

}
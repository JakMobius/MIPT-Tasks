//
// Created by Темыч on 20.04.2021.
//

#include "block_definition.hpp"

namespace bonk {

tree_node_block_definition* tree_node_block_definition::create() {
    DEFAULT_ALLOC_CONSTRUCT(tree_node_block_definition)
}

void tree_node_block_definition::construct() {
    ((tree_node*) this)->construct();

    body = nullptr;
    block_name = nullptr;

    type = TREE_NODE_TYPE_BLOCK_DEFINITION;
    vtable = &tree_node_block_definition::methods;
}

void tree_node_block_definition::virtual_destruct() {
    if(body) body->release();
    if(block_name) block_name->release();

    body = nullptr;
    block_name = nullptr;
}

void tree_node_block_definition::virtual_serialize(json_serializer* serializer) {
    (*this.*tree_node::methods.serialize)(serializer);

    serializer->block_string_field("type", "block_definition");
    serializer->block_string_field("block_name", block_name ? block_name->variable_name : nullptr);
    if(body) {
        serializer->block_start_block("body");
        body->serialize(serializer);
        serializer->close_block();
    } else {
        serializer->block_string_field("body", nullptr);
    }
}

const tree_node_methods tree_node_block_definition::methods = inherit(tree_node::methods, tree_node_methods {
    .destruct = (void (tree_node::*)()) &tree_node_block_definition::virtual_destruct,
    .serialize = (void (tree_node::*)(json_serializer*)) &tree_node_block_definition::virtual_serialize
});

}
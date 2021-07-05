//
// Created by Темыч on 20.04.2021.
//

#include "check.hpp"

namespace bonk {

tree_node_check* tree_node_check::create() {
    DEFAULT_ALLOC_CONSTRUCT(tree_node_check)
}

void tree_node_check::construct() {
    ((tree_node*) this)->construct();

    condition = nullptr;
    check_body = nullptr;
    or_body = nullptr;

    type = TREE_NODE_TYPE_CHECK;
    vtable = &methods;
}

void tree_node_check::virtual_destruct() {
    if(condition) condition->release();
    if(check_body) check_body->release();
    if(or_body) or_body->release();

    condition = nullptr;
    check_body = nullptr;
    or_body = nullptr;
}

void tree_node_check::virtual_serialize(json_serializer* serializer) {
    (*this.*tree_node::methods.serialize)(serializer);

    serializer->block_string_field("type", "check");

    if(check_body) {
        serializer->block_start_block("check_body");
        check_body->serialize(serializer);
        serializer->close_block();
    } else {
        serializer->block_string_field("parameter_value", nullptr);
    }

    if(or_body) {
        serializer->block_start_block("check_body");
        or_body->serialize(serializer);
        serializer->close_block();
    } else {
        serializer->block_string_field("or_body", nullptr);
    }

    if(condition) {
        serializer->block_start_block("condition");
        condition->serialize(serializer);
        serializer->close_block();
    } else {
        serializer->block_string_field("condition", nullptr);
    }
}

const tree_node_methods tree_node_check::methods = inherit(tree_node::methods, tree_node_methods {
    .destruct = (void (tree_node::*)()) &tree_node_check::virtual_destruct,
    .serialize = (void (tree_node::*)(json_serializer*)) &tree_node_check::virtual_serialize
});


}
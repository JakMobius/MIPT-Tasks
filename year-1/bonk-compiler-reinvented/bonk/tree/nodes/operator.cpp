//
// Created by Темыч on 20.04.2021.
//

#include "operator.hpp"

namespace bonk {

const char* OPERATOR_TYPE_NAMES[] = {
    "PLUS",
    "MINUS",
    "MULTIPLY",
    "DIVIDE",
    "ASSIGNMENT",
    "EQUALS",
    "NOT_EQUAL",
    "LESS_THAN",
    "GREATER_THAN",
    "LESS_OR_EQUAL_THAN",
    "GREATER_OR_EQUAL_THAN",
    "CYCLE",
    "CHECK",
    "PRINT",
    "BONK",
    "BREK",
    "BAMS",
    "AND",
    "OR",
    "REBONK",
    "INVALID"
};

tree_node_operator* tree_node_operator::create(operator_type oper) {
    DEFAULT_ALLOC_CONSTRUCT(tree_node_operator, oper)
}

void tree_node_operator::construct(operator_type oper) {
    ((tree_node*) this)->construct();

    oper_type = oper;
    type = TREE_NODE_TYPE_OPERATOR;
    vtable = &methods;
}

void tree_node_operator::virtual_serialize(json_serializer* serializer) {
    (*this.*tree_node::methods.serialize)(serializer);

    serializer->block_string_field("type", "operator");
    serializer->block_string_field("operator_type", OPERATOR_TYPE_NAMES[oper_type]);

    if(left) {
        serializer->block_start_block("left");
        left->serialize(serializer);
        serializer->close_block();
    } else {
        serializer->block_string_field("left", nullptr);
    }
    if(right) {
        serializer->block_start_block("right");
        right->serialize(serializer);
        serializer->close_block();
    } else {
        serializer->block_string_field("right", nullptr);
    }
}

const tree_node_methods tree_node_operator::methods = inherit(tree_node::methods, tree_node_methods {
    .serialize = (void (tree_node::*)(json_serializer*)) &tree_node_operator::virtual_serialize
});

}
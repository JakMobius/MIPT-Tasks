//
// Created by Темыч on 20.04.2021.
//

#include "call_parameter.hpp"

namespace bonk {

tree_node_call_parameter* tree_node_call_parameter::create(tree_node_identifier* name, tree_node* value) {
    DEFAULT_ALLOC_CONSTRUCT(tree_node_call_parameter, name, value)
}

void tree_node_call_parameter::construct(tree_node_identifier* name, tree_node* value) {
    ((tree_node*) this)->construct();

    parameter_name = name;
    parameter_value = value;
    type = TREE_NODE_TYPE_CALL_PARAMETER;
    vtable = &methods;
}

void tree_node_call_parameter::virtual_destruct() {
    if(parameter_name) parameter_name->release();
    if(parameter_value) parameter_value->release();
    parameter_name = nullptr;
    parameter_value = nullptr;
}

void tree_node_call_parameter::virtual_serialize(json_serializer* serializer) {
    (*this.*tree_node::methods.serialize)(serializer);

    serializer->block_string_field("type", "call_parameter");
    if(parameter_name) {
        serializer->block_start_block("parameter_name");
        parameter_name->serialize(serializer);
        serializer->close_block();
    } else {
        serializer->block_string_field("parameter_name", nullptr);
    }

    if(parameter_value) {
        serializer->block_start_block("parameter_value");
        parameter_value->serialize(serializer);
        serializer->close_block();
    } else {
        serializer->block_string_field("parameter_value", nullptr);
    }
}

const tree_node_methods tree_node_call_parameter::methods = inherit(tree_node::methods, tree_node_methods {
    .destruct = (void (tree_node::*)()) &tree_node_call_parameter::virtual_destruct,
    .serialize = (void (tree_node::*)(json_serializer*)) &tree_node_call_parameter::virtual_serialize
});

}
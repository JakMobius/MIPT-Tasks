//
// Created by Темыч on 21.04.2021.
//

#include "call.hpp"

namespace bonk {

tree_node_call* tree_node_call::create(tree_node_identifier* function, tree_node_list<tree_node_call_parameter*>* parameters) {
    DEFAULT_ALLOC_CONSTRUCT(tree_node_call, function, parameters)
}

void tree_node_call::construct(tree_node_identifier* function, tree_node_list<tree_node_call_parameter*>* parameters) {
    ((tree_node*) this)->construct();

    call_function = function;
    call_parameters = parameters;
    type = TREE_NODE_TYPE_CALL;
    vtable = &methods;
}

void tree_node_call::virtual_destruct() {
    if(call_function) call_function->release();
    if(call_parameters) call_parameters->release();
    call_function = nullptr;
    call_parameters = nullptr;
}

void tree_node_call::virtual_serialize(json_serializer* serializer) {
    (*this.*tree_node::methods.serialize)(serializer);
    serializer->block_string_field("type", "call");

    if(call_function) {
        serializer->block_start_block("call_function");
        call_function->serialize(serializer);
        serializer->close_block();
    } else {
        serializer->block_string_field("call_function", nullptr);
    }

    if(call_parameters) {
        serializer->block_start_block("call_parameters");
        call_parameters->serialize(serializer);
        serializer->close_block();
    } else {
        serializer->block_string_field("call_parameters", nullptr);
    }
}

const tree_node_methods tree_node_call::methods = inherit(tree_node::methods, tree_node_methods {
    .destruct = (void (tree_node::*)()) &tree_node_call::virtual_destruct,
    .serialize = (void (tree_node::*)(json_serializer*)) &tree_node_call::virtual_serialize
});

}
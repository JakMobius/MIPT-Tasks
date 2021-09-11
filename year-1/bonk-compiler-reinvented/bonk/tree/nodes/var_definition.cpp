//
// Created by Темыч on 20.04.2021.
//

#include "var_definition.hpp"

namespace bonk {

tree_node_variable_definition* tree_node_variable_definition::create(bool contextual, tree_node_identifier* identifier) {
    DEFAULT_ALLOC_CONSTRUCT(tree_node_variable_definition, contextual, identifier)
}

void tree_node_variable_definition::construct(bool contextual, tree_node_identifier* identifier) {
    ((tree_node*) this)->construct();

    variable_name = identifier;
    is_contextual = contextual;
    variable_value = nullptr;
    type = TREE_NODE_TYPE_VAR_DEFINITION;
    vtable = &methods;
}

void tree_node_variable_definition::virtual_serialize(json_serializer* serializer) {
    (*this.*tree_node::methods.serialize)(serializer);

    serializer->block_string_field("type", "var_definition");
    if(variable_name) {
        serializer->block_start_block("variable_name");
        variable_name->serialize(serializer);
        serializer->close_block();
    } else {
        serializer->block_string_field("variable_name", nullptr);
    }

    if(variable_value) {
        serializer->block_start_block("variable_value");
        variable_value->serialize(serializer);
        serializer->close_block();
    } else {
        serializer->block_string_field("variable_value", nullptr);
    }
}

void tree_node_variable_definition::virtual_destroy() {
    if(variable_name) variable_name->release();
    if(variable_value) variable_value->release();

    variable_name = nullptr;
    variable_value = nullptr;
}

const tree_node_methods tree_node_variable_definition::methods = inherit(tree_node::methods, tree_node_methods {
    .destruct = (void (tree_node::*)()) &tree_node_variable_definition::virtual_destruct,
    .serialize = (void (tree_node::*)(json_serializer*)) &tree_node_variable_definition::virtual_serialize
});

}
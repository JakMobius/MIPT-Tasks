//
// Created by Темыч on 20.04.2021.
//

#include "identifier.hpp"

namespace bonk {

tree_node_identifier* tree_node_identifier::create(const char* name, unsigned long length) {
    DEFAULT_ALLOC_CONSTRUCT(tree_node_identifier, name, length)
}

void tree_node_identifier::construct(const char* name, unsigned long length) {
    ((tree_node*) this)->construct();

    variable_name = strndup(name, length);
    name_length = length;
    type = TREE_NODE_TYPE_IDENTIFIER;
    vtable = &methods;
}

bool tree_node_identifier::contents_equal(tree_node_identifier* other) {
    if(name_length != other->name_length) return false;
    return strcmp(variable_name, other->variable_name) == 0;
}

void tree_node_identifier::print(FILE* file) {
    fputs(variable_name, file);
}

void tree_node_identifier::virtual_destruct() {
    free((void*) variable_name);
}

void tree_node_identifier::virtual_serialize(json_serializer* serializer) {
    (*this.*tree_node::methods.serialize)(serializer);

    serializer->block_string_field("type", "identifier");
    serializer->block_string_field("identifier", variable_name);
}

const tree_node_methods tree_node_identifier::methods = inherit(tree_node::methods, tree_node_methods {
    .destruct = (void (tree_node::*)()) &tree_node_identifier::virtual_destruct,
    .serialize = (void (tree_node::*)(json_serializer*)) &tree_node_identifier::virtual_serialize
});

}
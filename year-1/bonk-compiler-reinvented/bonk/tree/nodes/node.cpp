//
// Created by Темыч on 20.04.2021.
//

#include "node.hpp"

namespace bonk {

tree_node* tree_node::create() {
    DEFAULT_ALLOC_CONSTRUCT(tree_node);
}

void tree_node::construct() {
    vtable = &methods;
}

void tree_node::virtual_destruct() {

}

void tree_node::virtual_serialize(json_serializer* serializer) {
    if(source_position) {
        const char* position_string = source_position->to_string();
        serializer->block_string_field("source_position", position_string);
        free((void*) position_string);
    }
}

void tree_node::release() {
    destruct();
    free(this);
}

/* Virtual method trampolines */
void tree_node::destruct() { (*this.*vtable->destruct)(); }

void tree_node::serialize(json_serializer* file) { (*this.*vtable->serialize)(file); }

const tree_node_methods tree_node::methods = {
    .destruct = &tree_node::virtual_destruct,
    .serialize = &tree_node::virtual_serialize
};

};
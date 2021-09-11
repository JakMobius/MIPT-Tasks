//
// Created by Темыч on 20.04.2021.
//

#include "number.hpp"

namespace bonk {

tree_node_number* tree_node_number::create(long double float_value, long long integer_value) {
    DEFAULT_ALLOC_CONSTRUCT(tree_node_number, float_value, integer_value)
}

void tree_node_number::construct(long double the_float_value, long long int the_integer_value) {
    ((tree_node*) this)->construct();

    float_value = the_float_value;
    integer_value = the_integer_value;
    type = TREE_NODE_TYPE_NUMBER;
    vtable = &methods;
}

void tree_node_number::virtual_serialize(json_serializer* serializer) {
    (*this.*tree_node::methods.serialize)(serializer);

    serializer->block_string_field("type", "root_list");
    serializer->block_number_field("float_value", float_value);
    serializer->block_number_field("integer_value", integer_value);
}

const tree_node_methods tree_node_number::methods = inherit(tree_node::methods, tree_node_methods {
        .serialize = (void (tree_node::*)(json_serializer*)) &tree_node_number::virtual_serialize
});


}
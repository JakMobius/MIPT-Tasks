//
// Created by Темыч on 20.04.2021.
//

#include "list.hpp"

#ifndef BONK_COMPILER_REINVENTED_TREE_NODE_LIST_CPP
#define BONK_COMPILER_REINVENTED_TREE_NODE_LIST_CPP

namespace bonk {

template<typename T>
struct tree_node_list : tree_node {

    mlist<T> list;

    static tree_node_list <T>* create();
    bool construct();

    /* Override virtual methods */

    void virtual_serialize(json_serializer* file);
    void virtual_destroy();

    static const tree_node_methods methods;
};

}

namespace bonk {

template<typename T>
tree_node_list<T>* tree_node_list<T>::create() {
    CHECK_ALLOC_CONSTRUCT(tree_node_list<T>)
}

template<typename T>
bool tree_node_list<T>::construct() {
    ((tree_node*) this)->construct();

    vtable = &methods;
    type = TREE_NODE_TYPE_LIST;
    return list.construct(4);
}

template<typename T>
void tree_node_list<T>::virtual_destroy() {
    for(auto i = list.begin(); i != list.end(); list.next_iterator(&i)) {
        list.get(i)->release();
    }
    list.destruct();
}

template<typename T>
void tree_node_list<T>::virtual_serialize(json_serializer* serializer) {
    (*this.*tree_node::methods.serialize)(serializer);

    serializer->block_string_field("type", "list");
    serializer->block_start_array("contents");

    for(auto i = list.begin(); i != list.end(); list.next_iterator(&i)) {
        auto* element = list.get(i);
        if(element) {
            serializer->array_add_block();
            element->serialize(serializer);
            serializer->close_block();
        } else serializer->array_add_string(nullptr);
    }

    serializer->close_array();
}

template<typename T>
const tree_node_methods tree_node_list<T>::methods = inherit(tree_node::methods, tree_node_methods {
    .destruct = (void (tree_node::*)()) &tree_node_list<T>::virtual_destroy,
    .serialize = (void (tree_node::*)(json_serializer*)) &tree_node_list<T>::virtual_serialize
});

}

#endif
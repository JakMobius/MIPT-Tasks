//
// Created by Темыч on 20.04.2021.
//

#ifndef BONK_COMPILER_REINVENTED_IDENTIFIER_HPP
#define BONK_COMPILER_REINVENTED_IDENTIFIER_HPP

#include "node.hpp"
#include "../../../utils/inherit.cpp"
#include "../../../utils/json_serializer.hpp"
#include <cstdlib>
#include <cstdio>

namespace bonk {

struct tree_node_identifier : tree_node {
    const char* variable_name;
    unsigned long name_length;

    static tree_node_identifier* create(const char* name, unsigned long length);
    void construct(const char* name, unsigned long length);

    bool contents_equal(tree_node_identifier* other);

    void print(FILE* file);

    /* Override methods */
    void virtual_destruct();
    void virtual_serialize(json_serializer* file);

    static const tree_node_methods methods;
};

}

#endif //BONK_COMPILER_REINVENTED_IDENTIFIER_HPP

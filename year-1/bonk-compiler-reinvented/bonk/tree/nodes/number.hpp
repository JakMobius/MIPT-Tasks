//
// Created by Темыч on 20.04.2021.
//

#ifndef BONK_COMPILER_REINVENTED_EDE_NUMBER_HPP
#define BONK_COMPILER_REINVENTED_EDE_NUMBER_HPP

namespace bonk {

struct tree_node_number;

}


#include "node.hpp"
#include "../../../utils/inherit.cpp"
#include "../../../utils/json_serializer.hpp"

namespace bonk {

struct tree_node_number : tree_node {
    long long integer_value;
    long double float_value;

    static tree_node_number* create(long double float_value, long long integer_value);
    void construct(long double float_value, long long integer_value);

    void virtual_serialize(json_serializer* file);

    static const tree_node_methods methods;
};

}

#endif //BONK_COMPILER_REINVENTED_EDE_NUMBER_HPP

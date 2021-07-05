//
// Created by Темыч on 20.04.2021.
//

#ifndef BONK_COMPILER_REINVENTED_OPERATOR_HPP
#define BONK_COMPILER_REINVENTED_OPERATOR_HPP

namespace bonk {

struct tree_node_operator;

enum operator_type {
    BONK_OPERATOR_PLUS,
    BONK_OPERATOR_MINUS,
    BONK_OPERATOR_MULTIPLY,
    BONK_OPERATOR_DIVIDE,
    BONK_OPERATOR_ASSIGNMENT,
    BONK_OPERATOR_EQUALS,
    BONK_OPERATOR_NOT_EQUAL,
    BONK_OPERATOR_LESS_THAN,
    BONK_OPERATOR_GREATER_THAN,
    BONK_OPERATOR_LESS_OR_EQUAL_THAN,
    BONK_OPERATOR_GREATER_OR_EQUAL_THAN,
    BONK_OPERATOR_CYCLE,
    BONK_OPERATOR_CHECK,
    BONK_OPERATOR_PRINT,
    BONK_OPERATOR_BONK,
    BONK_OPERATOR_BREK,
    BONK_OPERATOR_BAMS,
    BONK_OPERATOR_AND,
    BONK_OPERATOR_OR,
    BONK_OPERATOR_REBONK,
    BONK_OPERATOR_INVALID
};

extern const char* OPERATOR_TYPE_NAMES[];

}

#include "node.hpp"
#include "../../../utils/inherit.cpp"
#include "../../../utils/json_serializer.hpp"

namespace bonk {

struct tree_node_operator : tree_node {
    operator_type oper_type;
    tree_node* left;
    tree_node* right;

    static tree_node_operator* create(operator_type oper);
    void construct(operator_type oper);

    static const tree_node_methods methods;

    void virtual_serialize(json_serializer* file);
};

}

#endif //BONK_COMPILER_REINVENTED_OPERATOR_HPP


//
// Created by Темыч on 21.04.2021.
//

#ifndef BONK_COMPILER_REINVENTED_BACKEND_HPP
#define BONK_COMPILER_REINVENTED_BACKEND_HPP

namespace bonk {

struct backend;

}

#include "../compiler.hpp"

namespace bonk {

struct backend_method_table {
    bool (backend::*compile_ast)(compiler*, tree_node_list<tree_node*>*, FILE*);
    void (backend::*destruct)();
};

struct backend {

    void destruct();
    void release();

    bool virtual_compile_ast(compiler* linked_compiler, tree_node_list<tree_node*>* ast, FILE* target);
    void virtual_destruct();

    const backend_method_table* vtable;

    static const backend_method_table methods;

    void compile_ast(compiler* linked_compiler, tree_node_list<tree_node*>* ast, FILE* target);
};

}

#endif //BONK_COMPILER_REINVENTED_BACKEND_HPP

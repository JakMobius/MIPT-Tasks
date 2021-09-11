//
// Created by Темыч on 21.04.2021.
//

#ifndef BONK_COMPILER_REINVENTED_X86_BACKEND_HPP
#define BONK_COMPILER_REINVENTED_X86_BACKEND_HPP

namespace bonk::x86_backend {

struct backend;

}

#include "../../tree/nodes/nodes.hpp"
#include "../../compiler.hpp"
#include "../backend.hpp"

namespace bonk::x86_backend {

struct backend : bonk::backend {

    static backend* create();
    bool construct();

    void destruct();
    void release();

    bool virtual_compile_ast(compiler* linked_compiler, tree_node_list<bonk::tree_node*>* ast, FILE* target);
    void virtual_destruct();

    static const backend_method_table methods;
};

}

#endif //BONK_COMPILER_REINVENTED_X86_BACKEND_HPP

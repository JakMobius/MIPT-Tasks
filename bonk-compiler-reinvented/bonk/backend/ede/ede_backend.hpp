//
// Created by Темыч on 21.04.2021.
//

#ifndef BONK_COMPILER_REINVENTED_EDE_BACKEND_HPP
#define BONK_COMPILER_REINVENTED_EDE_BACKEND_HPP

#include "../../../utils/default_alloc_construct.hpp"
#include "../../tree/nodes/nodes.hpp"
#include "ede_field_list.hpp"
#include "ede_scope_stack.hpp"
#include "../../compiler.hpp"
#include "../backend.hpp"
#include "ede_backend_context.h"

namespace bonk::ede_backend {

struct backend : bonk::backend {

    static backend* create();
    bool construct();

    bool virtual_compile_ast(compiler* linked_compiler, tree_node_list<bonk::tree_node*>* ast, FILE* target);

    static const backend_method_table methods;
};

}

#endif //BONK_COMPILER_REINVENTED_EDE_BACKEND_HPP

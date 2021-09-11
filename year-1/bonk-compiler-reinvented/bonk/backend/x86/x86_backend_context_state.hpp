//
// Created by Темыч on 03.05.2021.
//

#ifndef BONK_COMPILER_REINVENTED_X86_BACKEND_CONTEXT_STATE_HPP
#define BONK_COMPILER_REINVENTED_X86_BACKEND_CONTEXT_STATE_HPP

#include "x86_reg_stack.hpp"

namespace bonk::x86_backend {

struct backend_context_state;

}

namespace bonk::x86_backend {

struct backend_context_state {
    reg_stack register_stack;
    command_list* current_command_list;

    bool construct(command_list* current_command_list);
    void destruct();
};

}

#endif //BONK_COMPILER_REINVENTED_X86_BACKEND_CONTEXT_STATE_HPP

//
// Created by Темыч on 02.05.2021.
//

#ifndef BONK_COMPILER_REINVENTED_X86_XCHG_HPP
#define BONK_COMPILER_REINVENTED_X86_XCHG_HPP

namespace bonk::x86_backend {

struct xchg_command;

}

#include "x86_command.hpp"
#include "../x86_backend_context.hpp"
#include "../x86_abstract_register.hpp"
#include "../../../../utils/linear_allocator.hpp"
#include "../x86_command_encoder.hpp"

namespace bonk::x86_backend {

struct xchg_command : asm_command {
    static xchg_command* create_reg64_reg64(abstract_register target, abstract_register source, command_list* list);
    void construct_reg64_reg64(abstract_register target, abstract_register source);

    static xchg_command* create_reg64_mem64(abstract_register reg64, command_parameter_memory mem64, command_list* list);
    void construct_reg64_mem64(abstract_register reg64, command_parameter_memory mem64);

    static xchg_command* create_mem64_reg64(command_parameter_memory mem64, abstract_register reg64, command_list* list);
    void construct_mem64_reg64(command_parameter_memory mem64, abstract_register reg64);

    void to_bytes(command_encoder* buffer);
};

}

#endif //BONK_COMPILER_REINVENTED_X86_XCHG_HPP

//
// Created by Темыч on 25.04.2021.
//

#ifndef BONK_COMPILER_REINVENTED_X86_XOR_HPP
#define BONK_COMPILER_REINVENTED_X86_XOR_HPP

#include "../x86_abstract_register.hpp"
#include "../../../../utils/linear_allocator.hpp"
#include "x86_command.hpp"
#include "../x86_command_encoder.hpp"

namespace bonk::x86_backend {

struct xor_command : asm_command {
    static xor_command* create_reg64_reg64(abstract_register target, abstract_register source, command_list* list);
    void construct_reg64_reg64(abstract_register target, abstract_register source);

    void to_bytes(command_encoder* buffer);
};

}

#endif //BONK_COMPILER_REINVENTED_X86_XOR_HPP

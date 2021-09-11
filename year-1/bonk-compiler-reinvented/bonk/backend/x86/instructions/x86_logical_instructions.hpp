//
// Created by Темыч on 06.05.2021.
//

#ifndef BONK_COMPILER_REINVENTED_X86_LOGICAL_INSTRUCTIONS_HPP
#define BONK_COMPILER_REINVENTED_X86_LOGICAL_INSTRUCTIONS_HPP

#include "../x86_abstract_register.hpp"
#include "../../../../utils/linear_allocator.hpp"
#include "x86_command.hpp"
#include "../x86_command_encoder.hpp"

namespace bonk::x86_backend {

struct and_command : asm_command {
    static and_command* create_reg8_reg8(abstract_register target, abstract_register source, command_list* list);
    void construct_reg8_reg8(abstract_register target, abstract_register source);
    void to_bytes(command_encoder* buffer);
};

struct or_command : asm_command {
    static or_command* create_reg8_reg8(abstract_register target, abstract_register source, command_list* list);
    void construct_reg8_reg8(abstract_register target, abstract_register source);
    void to_bytes(command_encoder* buffer);
};

}

#endif //BONK_COMPILER_REINVENTED_X86_LOGICAL_INSTRUCTIONS_HPP

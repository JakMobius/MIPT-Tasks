//
// Created by Темыч on 29.04.2021.
//

#ifndef BONK_COMPILER_REINVENTED_X86_PUSH_HPP
#define BONK_COMPILER_REINVENTED_X86_PUSH_HPP

#include "x86_command.hpp"
#include "../x86_abstract_register.hpp"
#include "../x86_command_encoder.hpp"

namespace bonk::x86_backend {

struct push_command : asm_command {
    static push_command* create_reg64(abstract_register reg, command_list* list);
    void construct_reg64(abstract_register reg);
    void to_bytes(command_encoder* buffer);
};

}

#endif //BONK_COMPILER_REINVENTED_X86_PUSH_HPP

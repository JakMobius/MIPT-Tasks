//
// Created by Темыч on 29.04.2021.
//

#ifndef BONK_COMPILER_REINVENTED_X86_POP_HPP
#define BONK_COMPILER_REINVENTED_X86_POP_HPP

#include "x86_command.hpp"
#include "../x86_abstract_register.hpp"
#include "../x86_command_encoder.hpp"

namespace bonk::x86_backend {

struct pop_command : asm_command {
    static pop_command* create_reg64(abstract_register divider, command_list* list);
    void construct_reg64(abstract_register divider);
    void to_bytes(command_encoder* buffer);
};

}

#endif //BONK_COMPILER_REINVENTED_X86_POP_HPP

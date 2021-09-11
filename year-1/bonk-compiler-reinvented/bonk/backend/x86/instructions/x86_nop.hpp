//
// Created by Темыч on 06.05.2021.
//

#ifndef BONK_COMPILER_REINVENTED_X86_NOP_HPP
#define BONK_COMPILER_REINVENTED_X86_NOP_HPP

#include "../x86_abstract_register.hpp"
#include "../../../../utils/linear_allocator.hpp"
#include "x86_command.hpp"
#include "../x86_command_encoder.hpp"

namespace bonk::x86_backend {

struct nop_command : asm_command {
    static nop_command* create(command_list* list);

    void construct();

    void to_bytes(command_encoder* buffer);
};

}

#endif //BONK_COMPILER_REINVENTED_X86_NOP_HPP

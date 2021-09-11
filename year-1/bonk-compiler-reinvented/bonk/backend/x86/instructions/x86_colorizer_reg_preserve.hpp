//
// Created by Темыч on 08.05.2021.
//

#ifndef BONK_COMPILER_REINVENTED_X86_COLORIZER_REG_PRESERVE_HPP
#define BONK_COMPILER_REINVENTED_X86_COLORIZER_REG_PRESERVE_HPP

#include "x86_command.hpp"

namespace bonk::x86_backend {

struct reg_preserve_command : asm_command {

    static reg_preserve_command* create(const e_machine_register* registers_to_preserve, int amount, command_list* list);
    void construct(const e_machine_register* registers_to_preserve, int amount);

};

}

#endif //BONK_COMPILER_REINVENTED_X86_COLORIZER_REG_PRESERVE_HPP

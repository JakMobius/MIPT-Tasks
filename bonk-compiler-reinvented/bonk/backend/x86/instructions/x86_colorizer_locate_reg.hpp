//
// Created by Темыч on 09.05.2021.
//

#ifndef BONK_COMPILER_REINVENTED_X86_COLORIZER_LOCATE_REG_HPP
#define BONK_COMPILER_REINVENTED_X86_COLORIZER_LOCATE_REG_HPP

#include "../x86_machine_register.hpp"
#include "../x86_abstract_register.hpp"
#include "x86_command.hpp"

namespace bonk::x86_backend {

struct locate_reg_command: asm_command {
    static locate_reg_command* create_reg64_reg64(abstract_register reg, e_machine_register loc, command_list* list);
    void construct_reg64_reg64(abstract_register reg, e_machine_register loc);

    static locate_reg_command* create_reg64_stack(abstract_register reg, int stack_position, command_list* list);
    void create_reg64_stack(abstract_register reg, int stack_position);

    e_machine_register get_reg_location();

    int get_stack_location();
};

}

#endif //BONK_COMPILER_REINVENTED_X86_COLORIZER_LOCATE_REG_HPP

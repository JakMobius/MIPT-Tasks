//
// Created by Темыч on 08.05.2021.
//

#include "x86_colorizer_reg_preserve.hpp"

namespace bonk::x86_backend {


reg_preserve_command* reg_preserve_command::create(const e_machine_register* registers_to_preserve, int amount, command_list* list) {
    X86_COMMAND_ALLOC_CONSTRUCT(amount, amount, amount, reg_preserve_command, list, construct, registers_to_preserve, amount);
}

void reg_preserve_command::construct(const e_machine_register* registers_to_preserve, int amount) {
    type = COMMAND_COLORIZER_REG_PRESERVE;
    for(int i = 0; i < amount; i++) {
        auto reg = list->parent_buffer->descriptors.machine_register(registers_to_preserve[i]);
        set_parameter(i, command_parameter::create_register_64(reg));
        set_read_register(i, reg);
        set_write_register(i, reg);
    }
}
}
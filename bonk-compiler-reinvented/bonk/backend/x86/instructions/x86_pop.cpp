//
// Created by Темыч on 29.04.2021.
//

#include "x86_pop.hpp"
#include "../x86_command_encoder.hpp"

namespace bonk::x86_backend {

pop_command* pop_command::create_reg64(abstract_register reg, command_list* list) {
    X86_COMMAND_ALLOC_CONSTRUCT(1, 0, 1, pop_command, list, construct_reg64, reg);
}

void pop_command::construct_reg64(abstract_register reg) {
    type = COMMAND_POP;

    set_parameter(0, command_parameter::create_register_64(reg));
    set_write_register(0, reg);
}

void pop_command::to_bytes(command_encoder* buffer) {
    e_machine_register reg = get_mapped_register(get_parameter(0).reg);
    register_extensions rex = { .b = reg > 7 };
    if(rex.exist()) buffer->push(rex.get_byte());
    buffer->push(0x58 + (reg & 7));
}

}
//
// Created by Темыч on 29.04.2021.
//

#include "x86_push.hpp"

namespace bonk::x86_backend {

push_command* push_command::create_reg64(abstract_register reg, command_list* list) {
    X86_COMMAND_ALLOC_CONSTRUCT(1, 1, 0, push_command, list, construct_reg64, reg);
}

void push_command::construct_reg64(abstract_register reg) {
    type = COMMAND_PUSH;

    set_parameter(0, command_parameter::create_register_64(reg));
    set_read_register(0, reg);
}

void push_command::to_bytes(command_encoder* buffer) {
    e_machine_register reg = get_mapped_register(get_parameter(0).reg);
    register_extensions rex = { .b = reg > 7 };
    if(rex.exist()) buffer->push(rex.get_byte());
    buffer->push(0x50 + (reg & 7));
}

}
//
// Created by Темыч on 06.05.2021.
//

#include "x86_logical_instructions.hpp"

namespace bonk::x86_backend {

and_command* and_command::create_reg8_reg8(abstract_register target, abstract_register source, command_list* list) {
    X86_COMMAND_ALLOC_CONSTRUCT(2, 2, 1, and_command, list, construct_reg8_reg8, target, source);
}

void and_command::construct_reg8_reg8(abstract_register target, abstract_register source) {
    type = COMMAND_AND;

    set_parameter(0, command_parameter::create_register_8(target));
    set_parameter(1, command_parameter::create_register_8(source));

    set_read_register(0, source);
    set_read_register(1, target);
    set_write_register(0, target);
}

void and_command::to_bytes(command_encoder* buffer) {

    command_parameter target = get_parameter(0);
    command_parameter source = get_parameter(1);

    if(target.type == PARAMETER_TYPE_REG_8 && source.type == PARAMETER_TYPE_REG_8) {
        write_prefix_opcode_modrm_sib(buffer, 0x22, target, source);

        return;
    }

    return ((asm_command*)this)->to_bytes(buffer);
}

or_command* or_command::create_reg8_reg8(abstract_register target, abstract_register source, command_list* list) {
    X86_COMMAND_ALLOC_CONSTRUCT(2, 2, 1, or_command, list, construct_reg8_reg8, target, source);
}

void or_command::construct_reg8_reg8(abstract_register target, abstract_register source) {
    type = COMMAND_OR;

    set_parameter(0, command_parameter::create_register_8(target));
    set_parameter(1, command_parameter::create_register_8(source));

    set_read_register(0, source);
    set_read_register(1, target);
    set_write_register(0, target);
}

void or_command::to_bytes(command_encoder* buffer) {

    command_parameter target = get_parameter(0);
    command_parameter source = get_parameter(1);

    if(target.type == PARAMETER_TYPE_REG_8 && source.type == PARAMETER_TYPE_REG_8) {
        write_prefix_opcode_modrm_sib(buffer, 0x0A, target, source);

        return;
    }

    return ((asm_command*)this)->to_bytes(buffer);
}

}
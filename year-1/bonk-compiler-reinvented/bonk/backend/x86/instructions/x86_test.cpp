//
// Created by Темыч on 06.05.2021.
//

#include "x86_test.hpp"

namespace bonk::x86_backend {

test_command* test_command::create_reg64_reg64(abstract_register target, abstract_register source, command_list* list) {
    X86_COMMAND_ALLOC_CONSTRUCT(2, 2, 0, test_command, list, construct_reg64_reg64, target, source)
}

void test_command::construct_reg64_reg64(abstract_register target, abstract_register source) {
    type = COMMAND_TEST;

    set_parameter(0, command_parameter::create_register_64(target));
    set_parameter(1, command_parameter::create_register_64(source));

    set_read_register(0, source);
    set_read_register(1, target);
}

test_command* test_command::create_reg8_reg8(abstract_register target, abstract_register source, command_list* list) {
    X86_COMMAND_ALLOC_CONSTRUCT(2, 2, 0, test_command, list, construct_reg64_reg64, target, source)
}

void test_command::construct_reg8_reg8(abstract_register target, abstract_register source) {
    type = COMMAND_TEST;

    set_parameter(0, command_parameter::create_register_8(target));
    set_parameter(1, command_parameter::create_register_8(source));

    set_read_register(0, source);
    set_read_register(1, target);
}

void test_command::to_bytes(command_encoder* buffer) {

    command_parameter target = get_parameter(0);
    command_parameter source = get_parameter(1);

    if(target.type == PARAMETER_TYPE_REG_64 && source.type == PARAMETER_TYPE_REG_64) {
        write_prefix_opcode_modrm_sib(buffer, 0x85, source, target);

        return;
    }

    if(target.type == PARAMETER_TYPE_REG_8 && source.type == PARAMETER_TYPE_REG_8) {
        write_prefix_opcode_modrm_sib(buffer, 0x84, source, target);

        return;
    }

    return ((asm_command*)this)->to_bytes(buffer);
}

}
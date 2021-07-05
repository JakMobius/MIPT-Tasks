//
// Created by Темыч on 25.04.2021.
//

#include "x86_add.hpp"

namespace bonk::x86_backend {

add_command* add_command::create_reg64_reg64(abstract_register target, abstract_register source, command_list* list) {
    X86_COMMAND_ALLOC_CONSTRUCT(2, 2, 1, add_command, list, construct_reg64_reg64, target, source);
}

add_command* add_command::create_reg64_imm32(abstract_register reg64, uint32_t imm32, command_list* list) {
    X86_COMMAND_ALLOC_CONSTRUCT(2, 1, 1, add_command, list, construct_reg64_imm32, reg64, imm32);
}

void add_command::construct_reg64_reg64(abstract_register target, abstract_register source) {
    type = COMMAND_ADD;

    set_parameter(0, command_parameter::create_register_64(target));
    set_parameter(1, command_parameter::create_register_64(source));

    set_read_register(0, source);
    set_read_register(1, target);
    set_write_register(0, target);
}

void add_command::construct_reg64_imm32(abstract_register reg64, uint32_t imm32) {
    type = COMMAND_ADD;

    set_parameter(0, command_parameter::create_register_64(reg64));
    set_parameter(1, command_parameter::create_imm(imm32));

    set_read_register(0, reg64);
    set_write_register(0, reg64);
}

void add_command::to_bytes(command_encoder* buffer) {

    command_parameter target = get_parameter(0);
    command_parameter source = get_parameter(1);

    if(target.type == PARAMETER_TYPE_REG_64 && source.type == PARAMETER_TYPE_REG_64) {
        // Turned out that these arguments must be passed in reverse order.
        write_prefix_opcode_modrm_sib(buffer, 0x01, source, target);

        return;
    }

    if(target.type == PARAMETER_TYPE_REG_64 && source.type == PARAMETER_TYPE_IMM) {
        write_extended_opcode(buffer, 0x81, 0x00, target);
        for(int i = 0; i < sizeof(uint32_t); i++) {
            buffer->push(((char*)&source.imm)[i]);
        }
        return;
    }

    return ((asm_command*)this)->to_bytes(buffer);
}

}
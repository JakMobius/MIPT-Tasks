
#include "x86_mov.hpp"

namespace bonk::x86_backend {

mov_command* mov_command::create_reg64_reg64(abstract_register target, abstract_register source, command_list* list) {
    X86_COMMAND_ALLOC_CONSTRUCT(2, 1, 1, mov_command, list, construct_reg64_reg64, target, source);
}

asm_command* mov_command::create_reg64_imm64(abstract_register reg64, uint64_t imm64, command_list* list) {
    X86_COMMAND_ALLOC_CONSTRUCT(2, 0, 1, mov_command, list, construct_reg64_imm64, reg64, imm64);
}

asm_command* mov_command::create_reg64_mem64(abstract_register reg64, command_parameter_memory mem64, command_list* list) {
    X86_COMMAND_ALLOC_CONSTRUCT(2, mem64.register_amount(), 1, mov_command, list, construct_reg64_mem64, reg64, mem64);
}

asm_command* mov_command::create_mem64_reg64(command_parameter_memory mem64, abstract_register reg64, command_list* list) {
    X86_COMMAND_ALLOC_CONSTRUCT(2, mem64.register_amount() + 1, 0, mov_command, list, construct_mem64_reg64, mem64, reg64);
}

asm_command* mov_command::create_reg8_imm8(abstract_register reg8, uint8_t imm8, command_list* list) {
    X86_COMMAND_ALLOC_CONSTRUCT(2, 0, 1, mov_command, list, construct_reg8_imm8, reg8, imm8);
}

asm_command* mov_command::create_reg64_sym64(abstract_register reg64, command_parameter_symbol sym64, command_list* list) {
    X86_COMMAND_ALLOC_CONSTRUCT(2, 0, 1, mov_command, list, construct_reg64_sym64, reg64, sym64);
}

movzx_command* movzx_command::create_reg64_reg8(abstract_register target, abstract_register source, command_list* list) {
    X86_COMMAND_ALLOC_CONSTRUCT(2, 1, 1, movzx_command, list, construct_reg64_reg8, target, source);
}

asm_command* mov_command::create_sym64_reg64(command_parameter_symbol sym64, abstract_register reg64, command_list* list) {
    X86_COMMAND_ALLOC_CONSTRUCT(2, 1, 0, mov_command, list, construct_sym64_reg64, sym64, reg64);
}

void movzx_command::construct_reg64_reg8(abstract_register target, abstract_register source) {
    type = COMMAND_MOVZX;

    set_parameter(0, command_parameter::create_register_64(target));
    set_parameter(1, command_parameter::create_register_8(source));

    set_read_register(0, source);
    set_write_register(0, target);
}

void mov_command::construct_sym64_reg64(command_parameter_symbol sym64, abstract_register reg64) {
    type = COMMAND_MOV;

    set_parameter(0, command_parameter::create_symbol(sym64));
    set_parameter(1, command_parameter::create_register_64(reg64));
    set_read_register(0, reg64);
}

void mov_command::construct_reg64_sym64(abstract_register reg64, command_parameter_symbol sym64) {
    type = COMMAND_MOV;

    set_parameter(0, command_parameter::create_register_64(reg64));
    set_parameter(1, command_parameter::create_symbol(sym64));
    set_write_register(0, reg64);
}

void movzx_command::to_bytes(command_encoder* buffer) {

    command_parameter target = get_parameter(0);
    command_parameter source = get_parameter(1);

    if(target.type == PARAMETER_TYPE_REG_64 && source.type == PARAMETER_TYPE_REG_8) {
        write_prefix_longopcode_regrm_sib(buffer, 0x0F, 0xB6, source, target);

        return;
    }

    return ((asm_command*)this)->to_bytes(buffer);
}

void mov_command::construct_reg64_reg64(abstract_register target, abstract_register source) {
    type = COMMAND_MOV;

    set_parameter(0, command_parameter::create_register_64(target));
    set_parameter(1, command_parameter::create_register_64(source));

    set_read_register(0, source);
    set_write_register(0, target);
}

void mov_command::construct_reg64_imm64(abstract_register reg64, uint64_t imm64) {
    type = COMMAND_MOV;

    set_parameter(0, command_parameter::create_register_64(reg64));
    set_parameter(1, command_parameter::create_imm(imm64));

    set_write_register(0, reg64);
}

void mov_command::construct_reg64_mem64(abstract_register reg64, command_parameter_memory mem64) {
    type = COMMAND_MOV;

    set_parameter(0, command_parameter::create_register_64(reg64));
    set_parameter(1, command_parameter::create_memory(mem64));

    for(int i = mem64.register_amount() - 1; i >= 0; i--) {
        set_read_register(i, mem64.get_register(i));
    }

    set_write_register(0, reg64);
}

void mov_command::construct_mem64_reg64(command_parameter_memory mem64, abstract_register reg64) {
    type = COMMAND_MOV;

    set_parameter(0, command_parameter::create_memory(mem64));
    set_parameter(1, command_parameter::create_register_64(reg64));

    set_read_register(0, reg64);

    for(int i = mem64.register_amount() - 1; i >= 0; i--) {
        set_read_register(i + 1, mem64.get_register(i));
    }
}

void mov_command::construct_reg8_imm8(abstract_register reg8, uint8_t imm8) {
    type = COMMAND_MOV;

    set_parameter(0, command_parameter::create_register_8(reg8));
    set_parameter(1, command_parameter::create_imm(imm8));

    set_write_register(0, reg8);
}

void mov_command::to_bytes(command_encoder* buffer) {

    command_parameter target = get_parameter(0);
    command_parameter source = get_parameter(1);

    if(target.type == PARAMETER_TYPE_REG_64 && source.type == PARAMETER_TYPE_REG_64) {
        write_prefix_opcode_modrm_sib(buffer, 0x89, source, target);

        return;
    }

    if(target.type == PARAMETER_TYPE_REG_64 && source.type == PARAMETER_TYPE_IMM) {
        register_extensions rex = { .w = true, .b = target.reg > 7 };

        buffer->push(rex.get_byte());
        buffer->push(0xb8 + (target.reg & 0b111)); // movabs opcode
        for(int i = 0; i < sizeof(uint64_t); i++)
            buffer->push(((char*)&source.imm)[i]);

        return;
    }

    if(target.type == PARAMETER_TYPE_REG_64 && source.type == PARAMETER_TYPE_MEMORY) {
        write_prefix_opcode_modrm_sib(buffer, 0x8B, target, source);

        return;
    }

    if(target.type == PARAMETER_TYPE_MEMORY && source.type == PARAMETER_TYPE_REG_64) {
        write_prefix_opcode_modrm_sib(buffer, 0x89, source, target);

        return;
    }

    if(target.type == PARAMETER_TYPE_REG_8 && source.type == PARAMETER_TYPE_IMM) {
        register_extensions rex = { .b = target.reg > 7 };

        buffer->push(rex.get_byte());
        buffer->push(0xb0 + (target.reg & 0b111)); // mov r8 imm8 opcode
        for(int i = 0; i < sizeof(uint8_t); i++)
            buffer->push(((char*)&source.imm)[i]);

        return;
    }

    if(target.type == PARAMETER_TYPE_REG_64 && source.type == PARAMETER_TYPE_SYMBOL) {
        write_prefix_opcode_modrm_sib(buffer, 0x8B, target, source);

        return;
    }

    if(target.type == PARAMETER_TYPE_SYMBOL && source.type == PARAMETER_TYPE_REG_64) {
        write_prefix_opcode_modrm_sib(buffer, 0x89, source, target);

        return;
    }

    return ((asm_command*)this)->to_bytes(buffer);
}

}
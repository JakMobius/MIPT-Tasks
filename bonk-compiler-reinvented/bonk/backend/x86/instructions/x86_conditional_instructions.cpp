//
// Created by Темыч on 29.04.2021.
//

#include "x86_conditional_instructions.hpp"

namespace bonk::x86_backend {

void write_jump_placeholder(command_encoder* buffer, jmp_label* label) {
    unsigned long offset_position = buffer->size;
    for(int i = 0; i < 4; i++) buffer->push(0x00);
    unsigned long next_instruction_offset = buffer->size;

    buffer->request_emplace({
        .offset = offset_position,
        .relation = next_instruction_offset,
        .bytes = 4,
        .command = label
    });
}

void write_jump(command_encoder* buffer, char opcode, jmp_label* label) {
    buffer->push(opcode);
    write_jump_placeholder(buffer, label);
}

void write_jump(command_encoder* buffer, char opcode_a, char opcode_b, jmp_label* label) {
    buffer->push(opcode_a);
    buffer->push(opcode_b);
    write_jump_placeholder(buffer, label);
}

void general_jmp_command::set_label(jmp_label* label) {
    set_parameter(0, command_parameter::create_label(label));
}

jmp_label* general_jmp_command::get_label() {
    return get_parameter(0).label;
}

void general_jmp_command::invert_condition() {
    switch(type) {
        case COMMAND_JE: type = COMMAND_JNE; break;
        case COMMAND_JNE: type = COMMAND_JE; break;
        case COMMAND_JL: type = COMMAND_JNL; break;
        case COMMAND_JNL: type = COMMAND_JL; break;
        case COMMAND_JG: type = COMMAND_JNG; break;
        case COMMAND_JNG: type = COMMAND_JG; break;
        default: assert(!"Unconditional jumps may not be inverted");
    }
}

jmp_command* jmp_command::create(jmp_label* where, command_list* list) {
    X86_COMMAND_ALLOC_CONSTRUCT(1, 0, 0, jmp_command, list, construct, where);
}

void jmp_command::construct(jmp_label* where) {
    type = COMMAND_JMP;
    set_label(where);
}

void jmp_command::to_bytes(command_encoder* buffer) {

    command_parameter target = get_parameter(0);

    if(target.type == PARAMETER_TYPE_LABEL) {
        write_jump(buffer, 0xE9, target.label);
        return;
    }

    return ((asm_command*)this)->to_bytes(buffer);
}

je_command* je_command::create(jmp_label* where, command_list* list) {
    X86_COMMAND_ALLOC_CONSTRUCT(1, 0, 0, je_command, list, construct, where);
}

void je_command::construct(jmp_label* where) {
    type = COMMAND_JE;
    set_label(where);
}

void je_command::to_bytes(command_encoder* buffer) {

    command_parameter target = get_parameter(0);

    if(target.type == PARAMETER_TYPE_LABEL) {
        write_jump(buffer, 0x0F, 0x84, target.label);
        return;
    }

    return ((asm_command*)this)->to_bytes(buffer);
}

jne_command* jne_command::create(jmp_label* where, command_list* list) {
    X86_COMMAND_ALLOC_CONSTRUCT(1, 0, 0, jne_command, list, construct, where);
}

void jne_command::construct(jmp_label* where) {
    type = COMMAND_JNE;
    set_label(where);
}

void jne_command::to_bytes(command_encoder* buffer) {

    command_parameter target = get_parameter(0);

    if(target.type == PARAMETER_TYPE_LABEL) {
        write_jump(buffer, 0x0F, 0x85, target.label);
        return;
    }

    return ((asm_command*)this)->to_bytes(buffer);
}

jg_command* jg_command::create(jmp_label* where, command_list* list) {
    X86_COMMAND_ALLOC_CONSTRUCT(1, 0, 0, jg_command, list, construct, where);
}

void jg_command::construct(jmp_label* where) {
    type = COMMAND_JG;
    set_label(where);
}

void jg_command::to_bytes(command_encoder* buffer) {

    command_parameter target = get_parameter(0);

    if(target.type == PARAMETER_TYPE_LABEL) {
        write_jump(buffer, 0x0F, 0x8F, target.label);
        return;
    }

    return ((asm_command*)this)->to_bytes(buffer);
}

jng_command* jng_command::create(jmp_label* where, command_list* list) {
    X86_COMMAND_ALLOC_CONSTRUCT(1, 0, 0, jng_command, list, construct, where);
}

void jng_command::construct(jmp_label* where) {
    type = COMMAND_JNG;
    set_label(where);
}

void jng_command::to_bytes(command_encoder* buffer) {

    command_parameter target = get_parameter(0);

    if(target.type == PARAMETER_TYPE_LABEL) {
        write_jump(buffer, 0x0F, 0x8E, target.label);
        return;
    }

    return ((asm_command*)this)->to_bytes(buffer);
}

jl_command* jl_command::create(jmp_label* where, command_list* list) {
    X86_COMMAND_ALLOC_CONSTRUCT(1, 0, 0, jl_command, list, construct, where);
}

void jl_command::construct(jmp_label* where) {
    type = COMMAND_JL;
    set_label(where);
}

void jl_command::to_bytes(command_encoder* buffer) {

    command_parameter target = get_parameter(0);

    if(target.type == PARAMETER_TYPE_LABEL) {
        write_jump(buffer, 0x0F, 0x8C, target.label);
        return;
    }

    return ((asm_command*)this)->to_bytes(buffer);
}

jge_command* jge_command::create(jmp_label* where, command_list* list) {
    X86_COMMAND_ALLOC_CONSTRUCT(1, 0, 0, jge_command, list, construct, where);
}

void jge_command::construct(jmp_label* where) {
    type = COMMAND_JNL;
    set_label(where);
}

void jge_command::to_bytes(command_encoder* buffer) {

    command_parameter target = get_parameter(0);

    if(target.type == PARAMETER_TYPE_LABEL) {
        write_jump(buffer, 0x0F, 0x8D, target.label);
        return;
    }

    return ((asm_command*)this)->to_bytes(buffer);
}

sete_command* sete_command::create_reg8(abstract_register reg, command_list* list) {
    X86_COMMAND_ALLOC_CONSTRUCT(1, 1, 1, sete_command, list, construct_reg8, reg);
}

void sete_command::construct_reg8(abstract_register reg) {
    type = COMMAND_SETE;
    set_read_register(0, reg);
    set_write_register(0, reg);
    set_parameter(0, command_parameter::create_register_8(reg));
}

void sete_command::to_bytes(command_encoder* buffer) {
    command_parameter target = get_parameter(0);

    if(target.type == PARAMETER_TYPE_REG_8) {
        write_extended_longopcode(buffer, 0x0F, 0x94, 0, target);
        return;
    }

    return ((asm_command*)this)->to_bytes(buffer);
}

setne_command* setne_command::create_reg8(abstract_register reg, command_list* list) {
    X86_COMMAND_ALLOC_CONSTRUCT(1, 1, 1, setne_command, list, construct_reg8, reg);
}

void setne_command::construct_reg8(abstract_register reg) {
    type = COMMAND_SETNE;
    set_read_register(0, reg);
    set_write_register(0, reg);
    set_parameter(0, command_parameter::create_register_8(reg));
}

void setne_command::to_bytes(command_encoder* buffer) {
    command_parameter target = get_parameter(0);

    if(target.type == PARAMETER_TYPE_REG_8) {
        write_extended_longopcode(buffer, 0x0F, 0x95, 0, target);
        return;
    }

    return ((asm_command*)this)->to_bytes(buffer);
}

setg_command* setg_command::create_reg8(abstract_register reg, command_list* list) {
    X86_COMMAND_ALLOC_CONSTRUCT(1, 1, 1, setg_command, list, construct_reg8, reg);
}

void setg_command::construct_reg8(abstract_register reg) {
    type = COMMAND_SETG;
    set_read_register(0, reg);
    set_write_register(0, reg);
    set_parameter(0, command_parameter::create_register_8(reg));
}

void setg_command::to_bytes(command_encoder* buffer) {
    command_parameter target = get_parameter(0);

    if(target.type == PARAMETER_TYPE_REG_8) {
        write_extended_longopcode(buffer, 0x0F, 0x9F, 0, target);
        return;
    }

    return ((asm_command*)this)->to_bytes(buffer);
}

setng_command* setng_command::create_reg8(abstract_register reg, command_list* list) {
    X86_COMMAND_ALLOC_CONSTRUCT(1, 1, 1, setng_command, list, construct_reg8, reg);
}

void setng_command::construct_reg8(abstract_register reg) {
    type = COMMAND_SETNG;
    set_read_register(0, reg);
    set_write_register(0, reg);
    set_parameter(0, command_parameter::create_register_8(reg));
}

void setng_command::to_bytes(command_encoder* buffer) {
    command_parameter target = get_parameter(0);

    if(target.type == PARAMETER_TYPE_REG_8) {
        write_extended_longopcode(buffer, 0x0F, 0x9E, 0, target);
        return;
    }

    return ((asm_command*)this)->to_bytes(buffer);
}

setl_command* setl_command::create_reg8(abstract_register reg, command_list* list) {
    X86_COMMAND_ALLOC_CONSTRUCT(1, 1, 1, setl_command, list, construct_reg8, reg);
}

void setl_command::construct_reg8(abstract_register reg) {
    type = COMMAND_SETL;
    set_read_register(0, reg);
    set_write_register(0, reg);
    set_parameter(0, command_parameter::create_register_8(reg));
}

void setl_command::to_bytes(command_encoder* buffer) {
    command_parameter target = get_parameter(0);

    if(target.type == PARAMETER_TYPE_REG_8) {
        write_extended_longopcode(buffer, 0x0F, 0x9C, 0, target);
        return;
    }

    return ((asm_command*)this)->to_bytes(buffer);
}

setge_command* setge_command::create_reg8(abstract_register reg, command_list* list) {
    X86_COMMAND_ALLOC_CONSTRUCT(1, 1, 1, setge_command, list, construct_reg8, reg);
}

void setge_command::construct_reg8(abstract_register reg) {
    type = COMMAND_SETNL;
    set_read_register(0, reg);
    set_write_register(0, reg);
    set_parameter(0, command_parameter::create_register_8(reg));
}

void setge_command::to_bytes(command_encoder* buffer) {
    command_parameter target = get_parameter(0);

    if(target.type == PARAMETER_TYPE_REG_8) {
        write_extended_longopcode(buffer, 0x0F, 0x9D, 0, target);
        return;
    }

    return ((asm_command*)this)->to_bytes(buffer);
}
}
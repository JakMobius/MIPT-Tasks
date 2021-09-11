//
// Created by Темыч on 25.04.2021.
//

#ifndef BONK_COMPILER_REINVENTED_X86_MOV_HPP
#define BONK_COMPILER_REINVENTED_X86_MOV_HPP

namespace bonk::x86_backend {

struct mov_command;

}

#include "x86_command.hpp"
#include "../x86_backend_context.hpp"
#include "../x86_abstract_register.hpp"
#include "../../../../utils/linear_allocator.hpp"

namespace bonk::x86_backend {

struct mov_command : asm_command {
    static mov_command* create_reg64_reg64(abstract_register target, abstract_register source, command_list* list);
    void construct_reg64_reg64(abstract_register target, abstract_register source);

    static asm_command* create_reg64_imm64(abstract_register aRegister, uint64_t value, command_list* list);
    void construct_reg64_imm64(abstract_register reg64, uint64_t imm64);

    static asm_command* create_reg64_mem64(abstract_register reg64, command_parameter_memory mem64, command_list* list);
    void construct_reg64_mem64(abstract_register reg64, command_parameter_memory mem64);

    static asm_command* create_mem64_reg64(command_parameter_memory mem64, abstract_register reg64, command_list* list);
    void construct_mem64_reg64(command_parameter_memory mem64, abstract_register reg64);

    static asm_command* create_reg8_imm8(abstract_register reg8, uint8_t imm8, command_list* list);
    void construct_reg8_imm8(abstract_register reg8, uint8_t imm8);

    static asm_command* create_reg64_sym64(abstract_register reg64, command_parameter_symbol sym64, command_list* list);
    void construct_reg64_sym64(abstract_register reg64, command_parameter_symbol sym64);

    static asm_command* create_sym64_reg64(command_parameter_symbol sym64, abstract_register reg64, command_list* list);
    void construct_sym64_reg64(command_parameter_symbol sym64, abstract_register reg64);

    void to_bytes(command_encoder* buffer);
};

struct movzx_command : asm_command {
    static movzx_command* create_reg64_reg8(abstract_register target, abstract_register source, command_list* list);
    void construct_reg64_reg8(abstract_register target, abstract_register source);

    void to_bytes(command_encoder* buffer);
};

}

#endif //BONK_COMPILER_REINVENTED_X86_MOV_HPP

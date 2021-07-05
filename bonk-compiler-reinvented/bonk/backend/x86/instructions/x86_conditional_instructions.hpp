
#ifndef BONK_COMPILER_REINVENTED_X86_CONDITIONAL_INSTRUCTIONS_HPP
#define BONK_COMPILER_REINVENTED_X86_CONDITIONAL_INSTRUCTIONS_HPP

#include "x86_command.hpp"
#include "../x86_abstract_register.hpp"
#include "x86_jmp_label.hpp"

namespace bonk::x86_backend {

struct general_jmp_command : asm_command {
    void set_label(jmp_label* label);
    jmp_label* get_label();
    void invert_condition();
};

struct jmp_command : general_jmp_command {
    static jmp_command* create(jmp_label* where, command_list* list);
    void construct(jmp_label* where);
    void to_bytes(command_encoder* buffer);
};

struct je_command : general_jmp_command {
    static je_command* create(jmp_label* where, command_list* list);
    void construct(jmp_label* where);
    void to_bytes(command_encoder* buffer);
};

struct jne_command : general_jmp_command {
    static jne_command* create(jmp_label* where, command_list* list);
    void construct(jmp_label* where);
    void to_bytes(command_encoder* buffer);
};

struct jg_command : general_jmp_command {
    static jg_command* create(jmp_label* where, command_list* list);
    void construct(jmp_label* where);
    void to_bytes(command_encoder* buffer);
};

struct jng_command : general_jmp_command {
    static jng_command* create(jmp_label* where, command_list* list);
    void construct(jmp_label* where);
    void to_bytes(command_encoder* buffer);
};

struct jl_command : general_jmp_command {
    static jl_command* create(jmp_label* where, command_list* list);
    void construct(jmp_label* where);
    void to_bytes(command_encoder* buffer);
};

struct jge_command : general_jmp_command {
    static jge_command* create(jmp_label* where, command_list* list);
    void construct(jmp_label* where);
    void to_bytes(command_encoder* buffer);
};

struct sete_command : asm_command {
    static sete_command* create_reg8(abstract_register reg, command_list* list);
    void construct_reg8(abstract_register reg);
    void to_bytes(command_encoder* buffer);
};

struct setne_command : asm_command {
    static setne_command* create_reg8(abstract_register reg, command_list* list);
    void construct_reg8(abstract_register reg);
    void to_bytes(command_encoder* buffer);
};

struct setg_command : asm_command {
    static setg_command* create_reg8(abstract_register reg, command_list* list);
    void construct_reg8(abstract_register reg);
    void to_bytes(command_encoder* buffer);
};

struct setng_command : asm_command {
    static setng_command* create_reg8(abstract_register reg, command_list* list);
    void construct_reg8(abstract_register reg);
    void to_bytes(command_encoder* buffer);
};

struct setl_command : asm_command {
    static setl_command* create_reg8(abstract_register reg, command_list* list);
    void construct_reg8(abstract_register reg);
    void to_bytes(command_encoder* buffer);
};

struct setge_command : asm_command {
    static setge_command* create_reg8(abstract_register reg, command_list* list);
    void construct_reg8(abstract_register reg);
    void to_bytes(command_encoder* buffer);
};

}

#endif //BONK_COMPILER_REINVENTED_X86_CONDITIONAL_INSTRUCTIONS_HPP

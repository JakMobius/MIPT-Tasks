//
// Created by Темыч on 25.04.2021.
//

#ifndef BONK_COMPILER_REINVENTED_X86_COMMAND_HPP
#define BONK_COMPILER_REINVENTED_X86_COMMAND_HPP

namespace bonk::x86_backend {

struct asm_command;

enum asm_command_type {
    COMMAND_COLORIZER_SCOPE_DEAD_END,
    COMMAND_COLORIZER_SCOPE_POP,
    COMMAND_COLORIZER_SCOPE,
    COMMAND_COLORIZER_REPEAT_SCOPE,
    COMMAND_COLORIZER_REG_PRESERVE,
    COMMAND_COLORIZER_FRAME_CREATE,
    COMMAND_COLORIZER_FRAME_DESTROY,
    COMMAND_COLORIZER_LOCATE_REG_REG,
    COMMAND_COLORIZER_LOCATE_REG_STACK,
    COMMAND_COLORIZER_ALIGN_STACK_BEFORE,
    COMMAND_COLORIZER_ALIGN_STACK_AFTER,
    COMMAND_JMP_LABEL,
    COMMAND_MOV,
    COMMAND_ADD,
    COMMAND_SUB,
    COMMAND_IMUL,
    COMMAND_IDIV,
    COMMAND_XOR,
    COMMAND_PUSH,
    COMMAND_POP,
    COMMAND_JMP,
    COMMAND_JE,
    COMMAND_JNE,
    COMMAND_JG,
    COMMAND_JNG,
    COMMAND_JL,
    COMMAND_JNL,
    COMMAND_SETE,
    COMMAND_SETNE,
    COMMAND_SETG,
    COMMAND_SETNG,
    COMMAND_SETL,
    COMMAND_SETNL,
    COMMAND_XCHG,
    COMMAND_RET,
    COMMAND_MOVZX,
    COMMAND_CMP,
    COMMAND_TEST,
    COMMAND_AND,
    COMMAND_OR,
    COMMAND_NOP,
    COMMAND_CALL
};

extern const char* ASM_MNEMONICS[];
extern const char* ASM_REGISTERS_64[];
extern const char* ASM_REGISTERS_8[];

}

#define X86_COMMAND_ALLOC_CONSTRUCT(PARAMETERS, READ_REGISTERS, WRITE_REGISTERS, TYPE, LIST, CONSTRUCT_FUNC, ...) { \
    auto* command = (TYPE*) allocate_structure(PARAMETERS, READ_REGISTERS, WRITE_REGISTERS, sizeof(TYPE), &LIST->parent_buffer->allocator); \
    if(!command) return nullptr;                          \
    command->list = LIST;                             \
    command->CONSTRUCT_FUNC(__VA_ARGS__);                 \
    return command;                                       \
}

#include "../x86_command_buffer.hpp"
#include "../x86_abstract_register.hpp"
#include "../../../../utils/linear_allocator.hpp"
#include "../x86_register_extensions.hpp"
#include "../x86_command_encoder.hpp"

namespace bonk::x86_backend {

struct backend_context;

struct command_parameter_memory {
    int32_t displacement;
    abstract_register reg_a;
    abstract_register reg_b;
    uint8_t reg_a_constant;

    uint8_t register_amount();
    abstract_register get_register(uint8_t index);
    void set_register(uint8_t index, abstract_register reg);

    static command_parameter_memory create_reg(abstract_register reg);
    static command_parameter_memory create_displ(int32_t displacement);
    static command_parameter_memory create_reg_displ(abstract_register reg, int32_t displacement);
    static command_parameter_memory create_reg_reg(abstract_register reg_a, abstract_register reg_b);
    static command_parameter_memory create_reg_reg_displ(abstract_register reg_a, abstract_register reg_b, int32_t displacement);
    static command_parameter_memory create_reg_const_displ(abstract_register reg, uint8_t reg_a_constant, int32_t displacement);
    static command_parameter_memory create_reg_const_reg_displ(abstract_register reg_a, uint8_t reg_a_constant, abstract_register reg_b, int32_t displacement);
};

struct command_parameter_symbol {
    uint32_t symbol;
    bool ip_relative;

    static command_parameter_symbol create(bool ip_relative, uint32_t symbol);
};

enum command_parameter_type {
    PARAMETER_TYPE_UNSET,
    PARAMETER_TYPE_REG_64,
    PARAMETER_TYPE_REG_8,
    PARAMETER_TYPE_MEMORY,
    PARAMETER_TYPE_IMM,
    PARAMETER_TYPE_LABEL,
    PARAMETER_TYPE_SYMBOL
};

struct command_parameter {
    union {
        abstract_register reg;
        long long imm;
        jmp_label* label;
        command_parameter_memory memory;
        command_parameter_symbol symbol;
    };
    command_parameter_type type;

    static command_parameter create_register_64(abstract_register reg);
    static command_parameter create_register_8(abstract_register reg);
    static command_parameter create_imm(long long imm);
    static command_parameter create_label(jmp_label* label);
    static command_parameter create_memory(command_parameter_memory memory);
    static command_parameter create_symbol(command_parameter_symbol symbol);

};

struct asm_command {
    command_list* list;
    asm_command_type type;
    long offset;

    int structure_size;
    int total_read_registers;
    int total_write_registers;
    int total_parameters;

    static unsigned long long get_structure_bytes(int parameters, int read_registers, int write_registers, int structure_size);
    static void* allocate_structure(uint8_t parameters, uint8_t read_registers, uint8_t write_registers, uint8_t structure_size, linear_allocator* allocator);

    void set_read_register(int index, abstract_register reg);
    void set_write_register(int index, abstract_register reg);
    void set_parameter(int index, command_parameter reg);

    abstract_register get_read_register(int index);
    abstract_register get_write_register(int index);
    command_parameter get_parameter(int i);

    bool is_read_register(abstract_register reg);
    bool is_write_register(abstract_register reg);

    void dump(FILE* file, int depth);
    void to_bytes(command_encoder* buffer);

    void dump_register(FILE* file, abstract_register reg);
    void dump_param_register(FILE* file, command_parameter param);
    void depth_padding(FILE* file, int depth, bool is_label);

    e_machine_register get_mapped_register(abstract_register reg);

    asm_command* clone(command_list* list);

    bool is_sip(command_parameter reg_rm);
    char get_displacement_bytes(command_parameter reg_rm, register_extensions* extensions);
    char get_mod_reg_rm_byte(command_parameter reg, command_parameter reg_rm, register_extensions* extensions);
    char get_sib_byte(command_parameter reg_rm, register_extensions* extensions);
    uint64_t get_displacement(command_parameter rm);
    void get_sib_base_index(command_parameter reg_rm, char* index_p, char* base_p, bool* requires_mask);
    void write_prefix_opcode_modrm_sib(command_encoder* buffer, char opcode, command_parameter reg, command_parameter reg_rm);
    void write_prefix_longopcode_regrm_sib(command_encoder* buffer, char opcode_a, char opcode_b, command_parameter reg, command_parameter reg_rm);
    void write_extended_opcode(command_encoder* buffer, char opcode, char extension, command_parameter reg);
    void write_extended_longopcode(command_encoder* buffer, char opcode_a, char opcode_b, char extension, command_parameter reg);
};

};

#endif //BONK_COMPILER_REINVENTED_X86_COMMAND_HPP

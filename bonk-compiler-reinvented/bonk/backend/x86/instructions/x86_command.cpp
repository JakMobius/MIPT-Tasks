//
// Created by Темыч on 25.04.2021.
//

#include "x86_command.hpp"
#include "../x86_backend_context.hpp"

namespace bonk::x86_backend {

const char* ASM_MNEMONICS[] = {
    "colorizer::dead_end",
    "colorizer::scope_pop",
    "colorizer::scope",
    "colorizer::repeat_scope",
    "colorizer::reg_preserve",
    "colorizer::frame_create",
    "colorizer::frame_destroy",
    "colorizer::locate_register (in register)",
    "colorizer::locate_register (in stack)",
    "colorizer::align_stack (before)",
    "colorizer::align_stack (after)",
    "meta::jmp_label",
    "mov",
    "add",
    "sub",
    "imul",
    "idiv",
    "xor",
    "push",
    "pop",
    "jmp",
    "je",
    "jne",
    "jg",
    "jng",
    "jl",
    "jge",
    "sete",
    "setne",
    "setg",
    "setng",
    "setl",
    "setge",
    "xchg",
    "ret",
    "movzx",
    "cmp",
    "test",
    "and",
    "or",
    "nop",
    "call"
};

const char* ASM_REGISTERS_64[] = {
    "rax",
    "rcx",
    "rdx",
    "rbx",
    "rsp",
    "rbp",
    "rsi",
    "rdi",
    "r8",
    "r9",
    "r10",
    "r11",
    "r12",
    "r13",
    "r14",
    "r15"
};

const char* ASM_REGISTERS_8[] = {
    "al",
    "cl",
    "dl",
    "bl",
    "spl",
    "bpl",
    "sil",
    "dil",
    "r8b",
    "r9b",
    "r10b",
    "r11b",
    "r12b",
    "r13b",
    "r14b",
    "r15b"
};


unsigned long long asm_command::get_structure_bytes(int parameters, int read_registers, int write_registers, int structure_size) {
    return structure_size
         + read_registers * sizeof(abstract_register)
         + write_registers * sizeof(abstract_register)
         + parameters * sizeof(command_parameter);
}

void* asm_command::allocate_structure(uint8_t parameters, uint8_t read_registers, uint8_t write_registers, uint8_t structure_size, linear_allocator* allocator) {
    unsigned long long bytes = get_structure_bytes(parameters, read_registers, write_registers, structure_size);
    auto* command = (asm_command*) allocator->allocate(bytes);

    command->structure_size = structure_size;
    command->total_read_registers = read_registers;
    command->total_write_registers = write_registers;
    command->total_parameters = parameters;

    return command;
}

void asm_command::set_read_register(int index, abstract_register reg) {
    assert(index < total_read_registers);
    auto* read_register_storage = (abstract_register*)((char*)this + structure_size);
    read_register_storage[index] = reg;
}

void asm_command::set_write_register(int index, abstract_register reg) {
    assert(index < total_write_registers);
    auto* write_register_storage = (abstract_register*)((char*)this + structure_size) + total_read_registers;
    write_register_storage[index] = reg;
}

void asm_command::set_parameter(int index, command_parameter reg) {
    assert(index < total_parameters);
    auto* parameter_storage = (command_parameter*)((abstract_register*)((char*)this + structure_size) + total_read_registers + total_write_registers);
    command_parameter old_parameter = parameter_storage[index];
    if(old_parameter.type != PARAMETER_TYPE_UNSET) {
        assert(reg.type == old_parameter.type);
        if(reg.type == PARAMETER_TYPE_LABEL) {
            if(old_parameter.label) old_parameter.label->jmps_targeting--;
            if(reg.label) reg.label->jmps_targeting++;
        }
    }

    parameter_storage[index] = reg;
}

command_parameter asm_command::get_parameter(int i) {
    assert(i < total_parameters);
    auto* parameter_storage = (command_parameter*)((abstract_register*)((char*)this + structure_size) + total_read_registers + total_write_registers);
    return parameter_storage[i];
}

abstract_register asm_command::get_read_register(int index) {
    assert(index < total_read_registers);
    auto* read_register_storage = (abstract_register*)((char*)this + structure_size);
    return read_register_storage[index];
}

abstract_register asm_command::get_write_register(int index) {
    assert(index < total_write_registers);
    auto* write_register_storage = (abstract_register*)((char*)this + structure_size) + total_read_registers;
    return write_register_storage[index];
}

bool asm_command::is_read_register(abstract_register reg) {
    for(int i = 0; i < total_read_registers; i++) {
        if(get_read_register(i) == reg) return true;
    }
    return false;
}

bool asm_command::is_write_register(abstract_register reg) {
    for(int i = 0; i < total_write_registers; i++) {
        if(get_write_register(i) == reg) return true;
    }
    return false;
}

void asm_command::depth_padding(FILE* file, int depth, bool is_label) {
    if(!is_label) fprintf(file, "   ");
    for(int i = 0; i < depth; i++) { fprintf(file, "  "); }
}

void asm_command::dump_register(FILE* file, abstract_register reg) {
    abstract_register_descriptor* descriptor = list->parent_buffer->descriptors.get_descriptor(reg);
    if(descriptor->has_register_constraint) {
        fprintf(file, "%s", ASM_REGISTERS_64[descriptor->last_register_location]);
    } else {
        fprintf(file, "_r%llu", reg);
    }
}

void asm_command::dump_param_register(FILE* file, command_parameter param) {
    abstract_register_descriptor* descriptor = list->parent_buffer->descriptors.get_descriptor(param.reg);
    if(descriptor->has_register_constraint) {
        if(param.type == PARAMETER_TYPE_REG_64) {
            fprintf(file, "%s", ASM_REGISTERS_64[descriptor->last_register_location]);
        } else {
            fprintf(file, "%s", ASM_REGISTERS_8[descriptor->last_register_location]);
        }
    } else {
        fprintf(file, "_r%llu", param.reg);
    }
}

void asm_command::dump(FILE* file, int depth) {
    if(type == COMMAND_COLORIZER_SCOPE) {
        ((scope_command*)this)->dump(file, depth);
        return;
    }
    if(type == COMMAND_JMP_LABEL) {
        depth_padding(file, depth, true);
        fprintf(file, "L%lu:", ((jmp_label*)this)->get_index());
        return;
    }
    depth_padding(file, depth, false);
    fprintf(file, "%s", ASM_MNEMONICS[type]);
    for(int i = 0; i < total_parameters; i++) {
        if(i != 0) fputc(',', file);
        command_parameter parameter = get_parameter(i);
        if(parameter.type == PARAMETER_TYPE_LABEL) {
            if(parameter.label) {
                fprintf(file, " L%lu", parameter.label->get_index());
            } else {
                fprintf(file, " LNULL");
            }
        } else if(parameter.type == PARAMETER_TYPE_IMM) {
            fprintf(file, " %lld", parameter.imm);
        } else if(parameter.type == PARAMETER_TYPE_REG_64) {
            fputc(' ', file);
            dump_param_register(file, parameter);
        } else if(parameter.type == PARAMETER_TYPE_REG_8) {
            fputc(' ', file);
            dump_param_register(file, parameter);
        } else if(parameter.type == PARAMETER_TYPE_MEMORY) {
            command_parameter_memory mem = parameter.memory;

            fprintf(file, " [");

            bool pad = false;

            if(mem.reg_a != abstract_register(-1)) {
                fprintf(file, "%s", ASM_REGISTERS_64[mem.reg_a]);
                if(mem.reg_a_constant != 1) fprintf(file, "*%d", mem.reg_a_constant);
                pad = true;
            }

            if(mem.reg_b != abstract_register(-1)) {
                if(pad) fputc(' ', file);
                fprintf(file, "%s", ASM_REGISTERS_64[mem.reg_b]);
                pad = true;
            }

            if(mem.displacement != 0) {
                if(pad) fputc(' ', file);
                if(mem.displacement > 0) {
                    fprintf(file, "+ %d", mem.displacement);
                } else {
                    fprintf(file, "- %d", -mem.displacement);
                }
            }

            fprintf(file, "]");
        } else if(parameter.type == PARAMETER_TYPE_SYMBOL) {
            fprintf(file, " %s",  this->list->parent_buffer->file->get_symbol_name(parameter.symbol.symbol));
        }
    }
}

void asm_command::to_bytes(command_encoder* buffer) {
    assert(!"Command is not compilable");
}

// To understand following functions, see
// http://www.cs.loyola.edu/~binkley/371/Encoding_Real_x86_Instructions.html
// Chapters 5-11

bool asm_command::is_sip(command_parameter reg_rm) {
    if(reg_rm.type == PARAMETER_TYPE_MEMORY) {
        int registers = reg_rm.memory.register_amount();
        if(registers > 1 || registers == 0) return true;
        if((reg_rm.memory.reg_a & 0b111) == rsp) return true;
        if(reg_rm.memory.reg_a_constant != 1) return true;
        return false;
    } else {
        return false;
    }
}

char asm_command::get_displacement_bytes(command_parameter reg_rm, register_extensions* extensions) {
    assert(reg_rm.type == PARAMETER_TYPE_REG_64 || reg_rm.type == PARAMETER_TYPE_REG_8 || reg_rm.type == PARAMETER_TYPE_MEMORY || reg_rm.type == PARAMETER_TYPE_IMM || reg_rm.type == PARAMETER_TYPE_SYMBOL);

    if(reg_rm.type == PARAMETER_TYPE_REG_64 || reg_rm.type == PARAMETER_TYPE_REG_8) {
        return 0;
    } else if(reg_rm.type == PARAMETER_TYPE_IMM || reg_rm.type == PARAMETER_TYPE_SYMBOL) {
        // 32-bit Displacement-only mode
        return 4;
    }

    // PARAMETER_TYPE_MEMORY
    char size = 0;
    if(reg_rm.memory.displacement == 0) size = 0;
    else if(reg_rm.memory.displacement >= INT8_MIN && reg_rm.memory.displacement <= INT8_MAX) size = 1;
    else size = 4;

    if(is_sip(reg_rm)) {
        bool requires_mask = false;
        char base = 0;
        get_sib_base_index(reg_rm, nullptr, &base, &requires_mask);
        if(requires_mask) {
            if(size == 0) size = 1;
        } else if((base & 0b111) == rbp) size = 4; // Displacement-only
    } else {
        if(size == 0) {
            char reg = get_mapped_register(reg_rm.memory.get_register(0));
            // [rbp] addressing is unavailable.
            // [rbp + disp8] or [rbp + disp32] should be used instead
            if((reg & 0b111) == rbp) size = 1;
        }
    }
    return size;
}

char asm_command::get_mod_reg_rm_byte(command_parameter reg, command_parameter reg_rm, register_extensions* extensions) {

    assert(reg.type == PARAMETER_TYPE_REG_64 || reg.type == PARAMETER_TYPE_REG_8);
    assert(reg_rm.type == PARAMETER_TYPE_REG_64 || reg_rm.type == PARAMETER_TYPE_REG_8 || reg_rm.type == PARAMETER_TYPE_MEMORY || reg_rm.type == PARAMETER_TYPE_IMM || reg_rm.type == PARAMETER_TYPE_SYMBOL);

    char mod_mask = 0b00;
    char rm_mask = 0b000;

    if(reg_rm.type == PARAMETER_TYPE_REG_64 || reg_rm.type == PARAMETER_TYPE_REG_8) {
        mod_mask = 0b11;
        rm_mask = get_mapped_register(reg_rm.reg);
        if(rm_mask > 7) extensions->b = true;
        rm_mask &= 0b111;
    } else if(reg_rm.type == PARAMETER_TYPE_IMM || reg_rm.type == PARAMETER_TYPE_SYMBOL) {
        mod_mask = 0b00;
        rm_mask = 0b101;
    } else if(reg_rm.type == PARAMETER_TYPE_MEMORY) {
        if(reg_rm.memory.displacement == 0) mod_mask = 0b00;
        else if(reg_rm.memory.displacement >= INT8_MIN && reg_rm.memory.displacement <= INT8_MAX) mod_mask = 0b01;
        else mod_mask = 0b10;

        if(is_sip(reg_rm)) {
            // SIP mode
            rm_mask = 0b100;

            bool requires_mask = false;
            char base = 0;
            get_sib_base_index(reg_rm, nullptr, &base, &requires_mask);
            if(requires_mask) {
                if(mod_mask == 0b00) mod_mask = 0b01;
            } else if((base & 0b111) == rbp) {
                mod_mask = 0b00;
            }
        } else {
            rm_mask = get_mapped_register(reg_rm.memory.get_register(0));
            // [rbp] addressing is unavailable.
            // [rbp + disp8] should be used instead
            if((rm_mask & 0b111) == rbp && mod_mask == 0b00) mod_mask = 0b01;
        }
    }

    if(reg.reg > 7) extensions->r = true;
    if(rm_mask > 7) extensions->b = true;

    reg.reg &= 0b111;
    rm_mask &= 0b111;

    return (mod_mask << 6) |
           (reg.reg << 3) |
           (rm_mask << 0);
}

void asm_command::get_sib_base_index(command_parameter reg_rm, char* index_p, char* base_p, bool* requires_mask) {
    int register_amount = reg_rm.memory.register_amount();

    char index = 0;
    char base = 0;
    bool base_requires_mask = false;

    if(register_amount > 0) index = get_mapped_register(reg_rm.memory.get_register(0));
    else index = 0b100;
    if(register_amount > 1) {
        base = get_mapped_register(reg_rm.memory.get_register(1));
        base_requires_mask = (base & 0b111) == rbp;
    } else base = 0b101;

    if(base_requires_mask && (base & 0b111) != (index & 0b111) && (base & 0b111) != rsp && reg_rm.memory.reg_a_constant == 1) {
        // Try to turn registers in reverse order if it is possible
        char temp = index;
        index = base;
        base = temp;
        base_requires_mask = false;
    }

    if(register_amount > 0) {
        // rsp cannot be index register.
        if((index & 0b111) == rsp) {
            if(register_amount == 1) {
                if(index == rsp) {
                    assert(reg_rm.memory.reg_a_constant == 1);
                    base = index;
                    // in case index is extended, we should enforce it to
                    // use invalid rsp mode
                    index = rsp;
                }
            } else if(index == rsp && register_amount == 2) {
                // Try to exchange base and index registers
                // Making sure it's possible
                assert(reg_rm.memory.reg_a_constant == 1 && base != index);
                char temp = index;
                index = base;
                base = temp;
            }
            if(register_amount > 1 && (base & 0b111) == rbp) {
                base_requires_mask = true;
            }
        }
    }

    if(base_p) *base_p = base;
    if(index_p) *index_p = index;
    if(requires_mask) *requires_mask = base_requires_mask;
}

char asm_command::get_sib_byte(command_parameter reg_rm, register_extensions* extensions) {
    assert(reg_rm.type == PARAMETER_TYPE_MEMORY);

    char scale = 0;
    char index = 0;
    char base = 0;

    get_sib_base_index(reg_rm, &index, &base, nullptr);

    if(index > 7) extensions->x = true;
    if(base > 7) extensions->b = true;

    index &= 0b111;
    base &= 0b111;

    switch(reg_rm.memory.reg_a_constant) {
        case 1: scale = 0; break;
        case 2: scale = 1; break;
        case 4: scale = 2; break;
        case 8: scale = 4; break;
        default: assert(!"Invalid scale");
    }

    return  (scale << 6) |
            (index << 3) |
            (base << 0);
}

void asm_command::write_prefix_opcode_modrm_sib(command_encoder* buffer, char opcode, command_parameter reg, command_parameter reg_rm) {
    assert(reg.type == PARAMETER_TYPE_REG_64 || reg.type == PARAMETER_TYPE_REG_8);

    register_extensions extensions = { .w = reg.type == PARAMETER_TYPE_REG_64 };

    bool sib = is_sip(reg_rm);
    char mod_reg_rm = get_mod_reg_rm_byte(reg, reg_rm, &extensions);
    char sib_byte = sib ? get_sib_byte(reg_rm, &extensions) : 0;
    char displacement_size = get_displacement_bytes(reg_rm, &extensions);
    uint64_t displacement = get_displacement(reg_rm);

    if(extensions.exist()) buffer->push(extensions.get_byte());
    buffer->push(opcode);
    buffer->push(mod_reg_rm);
    if(sib) buffer->push(sib_byte);

    if(reg_rm.type == PARAMETER_TYPE_SYMBOL) {
        buffer->request_relocation(relocation_request {
                .relocation = reg_rm.symbol.symbol,
                .address = buffer->size,
                .type = macho::RELOCATION_TYPE_CONSTANT,
                .data_length = 4,
                .pc_rel = true,
        });
    }

    for(int i = 0; i < displacement_size; i++) {
        buffer->push(((char*)&displacement)[i]);
    }
}

void asm_command::write_prefix_longopcode_regrm_sib(command_encoder* buffer, char opcode_a, char opcode_b, command_parameter reg, command_parameter reg_rm) {
    assert(reg.type == PARAMETER_TYPE_REG_64 || reg.type == PARAMETER_TYPE_REG_8);
    register_extensions extensions = { .w = reg.type == PARAMETER_TYPE_REG_64 };

    bool sib = is_sip(reg_rm);
    char mod_reg_rm = get_mod_reg_rm_byte(reg, reg_rm, &extensions);
    char sib_byte = sib ? get_sib_byte(reg_rm, &extensions) : 0;
    char displacement_size = get_displacement_bytes(reg_rm, &extensions);
    uint64_t displacement = get_displacement(reg_rm);

    if(extensions.exist()) buffer->push(extensions.get_byte());
    buffer->push(opcode_a);
    buffer->push(opcode_b);
    buffer->push(mod_reg_rm);
    if(sib) buffer->push(sib_byte);

    for(int i = 0; i < displacement_size; i++) {
        buffer->push(((char*)&displacement)[i]);
    }
}

void asm_command::write_extended_opcode(command_encoder* buffer, char opcode, char extension, command_parameter reg) {
    assert(reg.type == PARAMETER_TYPE_REG_64 || reg.type == PARAMETER_TYPE_REG_8);

    register_extensions extensions = { .w = reg.type == PARAMETER_TYPE_REG_64 };
    if(reg.reg > 7) extensions.b = true;

    char opcode_extension = 0b11000000;
    opcode_extension |= extension << 3;
    opcode_extension |= reg.reg;

    if(extensions.exist()) buffer->push(extensions.get_byte());
    buffer->push(opcode);
    buffer->push(opcode_extension);
}

void asm_command::write_extended_longopcode(command_encoder* buffer, char opcode_a, char opcode_b, char extension, command_parameter reg) {
    assert(reg.type == PARAMETER_TYPE_REG_64 || reg.type == PARAMETER_TYPE_REG_8);

    register_extensions extensions = { .w = reg.type == PARAMETER_TYPE_REG_64 };
    if(reg.reg > 7) extensions.b = true;

    char opcode_extension = 0b11000000;
    opcode_extension |= extension << 3;
    opcode_extension |= reg.reg;

    if(extensions.exist()) buffer->push(extensions.get_byte());
    buffer->push(opcode_a);
    buffer->push(opcode_b);
    buffer->push(opcode_extension);
}

uint64_t asm_command::get_displacement(command_parameter rm) {
    assert(rm.type == PARAMETER_TYPE_REG_64 || rm.type == PARAMETER_TYPE_REG_8 || rm.type == PARAMETER_TYPE_REG_8 || rm.type == PARAMETER_TYPE_MEMORY || rm.type == PARAMETER_TYPE_IMM || rm.type == PARAMETER_TYPE_SYMBOL);

    if(rm.type == PARAMETER_TYPE_REG_64 || rm.type == PARAMETER_TYPE_REG_8) return 0;
    if(rm.type == PARAMETER_TYPE_MEMORY) return rm.memory.displacement;
    return rm.imm;
}

e_machine_register asm_command::get_mapped_register(abstract_register reg) {
    auto descriptor = list->parent_buffer->descriptors.get_descriptor(reg);
    assert(descriptor->has_register_constraint);
    return descriptor->last_register_location;
}

asm_command* asm_command::clone(command_list* list) {
    auto* new_command = (asm_command*)asm_command::allocate_structure(
            total_parameters,
            total_read_registers,
            total_write_registers,
            structure_size,
            &list->parent_buffer->allocator);

    new_command->list = list;
    new_command->type = type;

    for(int i = 0; i < total_parameters; i++) {
        command_parameter parameter = get_parameter(i);
        new_command->set_parameter(i, parameter);
    }

    for(int i = 0; i < total_read_registers; i++) {
        abstract_register reg = get_read_register(i);
        new_command->set_read_register(i, reg);
    }

    for(int i = 0; i < total_write_registers; i++) {
        abstract_register reg = get_write_register(i);
        new_command->set_write_register(i, reg);
    }

    return new_command;
}

command_parameter command_parameter::create_register_64(abstract_register reg) {
    return { .reg = reg, .type = PARAMETER_TYPE_REG_64 };
}

command_parameter command_parameter::create_register_8(abstract_register reg) {
    return { .reg = reg, .type = PARAMETER_TYPE_REG_8 };
}

command_parameter command_parameter::create_imm(long long imm) {
    return { .imm = imm, .type = PARAMETER_TYPE_IMM };
}

command_parameter command_parameter::create_label(jmp_label* label) {
    return { .label = label, .type = PARAMETER_TYPE_LABEL };
}

command_parameter command_parameter::create_memory(command_parameter_memory memory) {
    return { .memory = memory, .type = PARAMETER_TYPE_MEMORY };
}

command_parameter command_parameter::create_symbol(command_parameter_symbol symbol) {
    return { .symbol = symbol, .type = PARAMETER_TYPE_SYMBOL };
}

uint8_t command_parameter_memory::register_amount() {
    return (reg_a != abstract_register(-1)) + (reg_b != abstract_register(-1));
}

abstract_register command_parameter_memory::get_register(uint8_t index) {
    if(reg_a == abstract_register(-1)) return reg_b;
    if(index == 0) return reg_a;
    return reg_b;
}

void command_parameter_memory::set_register(uint8_t index, abstract_register reg) {
    if(reg_a == abstract_register(-1)) reg_b = reg;
    else if(index == 0) reg_a = reg;
    else reg_b = reg;
}

command_parameter_memory command_parameter_memory::create_reg(abstract_register reg) {
    return command_parameter_memory {
            .displacement = 0,
            .reg_a = reg,
            .reg_b = abstract_register(-1),
            .reg_a_constant = 1
    };
}

command_parameter_memory command_parameter_memory::create_reg_reg(abstract_register reg_a, abstract_register reg_b) {
    return command_parameter_memory {
        .displacement = 0,
        .reg_a = reg_a,
        .reg_b = reg_b,
        .reg_a_constant = 1
    };
}

command_parameter_memory command_parameter_memory::create_reg_displ(abstract_register reg, int32_t displacement) {
    return command_parameter_memory {
            .displacement = displacement,
            .reg_a = reg,
            .reg_b = abstract_register(-1),
            .reg_a_constant = 1
    };
}

command_parameter_memory command_parameter_memory::create_reg_reg_displ(abstract_register reg_a, abstract_register reg_b, int32_t displacement) {
    return command_parameter_memory {
            .displacement = displacement,
            .reg_a = reg_a,
            .reg_b = reg_b,
            .reg_a_constant = 1
    };
}

command_parameter_memory command_parameter_memory::create_reg_const_displ(abstract_register reg, uint8_t reg_a_constant, int32_t displacement) {
    return command_parameter_memory {
            .displacement = displacement,
            .reg_a = reg,
            .reg_b = abstract_register(-1),
            .reg_a_constant = reg_a_constant
    };
}

command_parameter_memory command_parameter_memory::create_reg_const_reg_displ(abstract_register reg_a, uint8_t reg_a_constant, abstract_register reg_b, int32_t displacement) {
    return command_parameter_memory {
        .displacement = displacement,
        .reg_a = reg_a,
        .reg_b = reg_b,
        .reg_a_constant = reg_a_constant
    };
}

command_parameter_memory command_parameter_memory::create_displ(int32_t displacement) {
    return command_parameter_memory {
        .displacement = displacement,
        .reg_a = abstract_register(-1),
        .reg_b = abstract_register(-1),
        .reg_a_constant = 1
    };
}

command_parameter_symbol command_parameter_symbol::create(bool ip_relative, uint32_t symbol) {
    return { .symbol = symbol, .ip_relative = ip_relative };
}
}
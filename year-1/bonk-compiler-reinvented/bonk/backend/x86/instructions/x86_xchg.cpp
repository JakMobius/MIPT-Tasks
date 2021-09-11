
#include "x86_xchg.hpp"
#include "../x86_command_encoder.hpp"

namespace bonk::x86_backend {

xchg_command* xchg_command::create_reg64_reg64(abstract_register target, abstract_register source, command_list* list) {
    X86_COMMAND_ALLOC_CONSTRUCT(2, 2, 2, xchg_command, list, construct_reg64_reg64, target, source);
}

xchg_command* xchg_command::create_reg64_mem64(abstract_register reg64, command_parameter_memory mem64, command_list* list) {
    X86_COMMAND_ALLOC_CONSTRUCT(2, mem64.register_amount() + 1, 1, xchg_command, list, construct_reg64_mem64, reg64, mem64);
}

xchg_command* xchg_command::create_mem64_reg64(command_parameter_memory mem64, abstract_register reg64, command_list* list) {
    X86_COMMAND_ALLOC_CONSTRUCT(2, mem64.register_amount() + 1, 1, xchg_command, list, construct_mem64_reg64, mem64, reg64);
}

void xchg_command::construct_reg64_reg64(abstract_register target, abstract_register source) {
    type = COMMAND_XCHG;

    set_parameter(0, command_parameter::create_register_64(target));
    set_parameter(1, command_parameter::create_register_64(source));

    set_read_register(0, source);
    set_read_register(1, target);
    set_write_register(0, source);
    set_write_register(0, target);
}

void xchg_command::construct_reg64_mem64(abstract_register reg64, command_parameter_memory mem64) {
    type = COMMAND_XCHG;

    set_parameter(0, command_parameter::create_register_64(reg64));
    set_parameter(1, command_parameter::create_memory(mem64));

    set_read_register(0, reg64);
    set_write_register(0, reg64);

    for(int i = mem64.register_amount() - 1; i >= 0; i--) {
        set_read_register(i + 1, mem64.get_register(i));
    }
}

void xchg_command::construct_mem64_reg64(command_parameter_memory mem64, abstract_register reg64) {
    type = COMMAND_XCHG;

    set_parameter(0, command_parameter::create_memory(mem64));
    set_parameter(1, command_parameter::create_register_64(reg64));

    set_read_register(0, reg64);
    set_write_register(0, reg64);

    for(int i = mem64.register_amount() - 1; i >= 0; i--) {
        set_read_register(i + 1, mem64.get_register(i));
    }
}

void xchg_command::to_bytes(command_encoder* buffer) {

    command_parameter target = get_parameter(0);
    command_parameter source = get_parameter(1);

    if(target.type == PARAMETER_TYPE_REG_64 && source.type == PARAMETER_TYPE_REG_64) {
        write_prefix_opcode_modrm_sib(buffer, 0x87, target, source);

        return;
    }

    if(target.type == PARAMETER_TYPE_REG_64 && source.type == PARAMETER_TYPE_MEMORY) {
        write_prefix_opcode_modrm_sib(buffer, 0x87, target, source);

        return;
    }

    if(target.type == PARAMETER_TYPE_MEMORY && source.type == PARAMETER_TYPE_REG_64) {
        write_prefix_opcode_modrm_sib(buffer, 0x87, source, target);

        return;
    }

    return ((asm_command*)this)->to_bytes(buffer);
}

}
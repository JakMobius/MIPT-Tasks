
#include "x86_xor.hpp"

namespace bonk::x86_backend {

xor_command* xor_command::create_reg64_reg64(abstract_register target, abstract_register source, command_list* list) {
    X86_COMMAND_ALLOC_CONSTRUCT(2, 2, 1, xor_command, list, construct_reg64_reg64, target, source)
}

void xor_command::construct_reg64_reg64(abstract_register target, abstract_register source) {
    type = COMMAND_XOR;

    set_parameter(0, command_parameter::create_register_64(target));
    set_parameter(1, command_parameter::create_register_64(source));

    set_read_register(0, source);
    set_read_register(1, target);

    set_write_register(0, target);
}

void xor_command::to_bytes(command_encoder* buffer) {

    command_parameter target = get_parameter(0);
    command_parameter source = get_parameter(1);

    if(target.type == PARAMETER_TYPE_REG_64 && source.type == PARAMETER_TYPE_REG_64) {
        write_prefix_opcode_modrm_sib(buffer, 0x31, source, target);

        return;
    }

    return ((asm_command*)this)->to_bytes(buffer);
}

}
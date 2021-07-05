//
// Created by Темыч on 25.04.2021.
//

#include "x86_idiv.hpp"

namespace bonk::x86_backend {

idiv_command* idiv_command::create_reg64(abstract_register divider, abstract_register rax_handle, abstract_register rdx_handle, command_list* list) {
    X86_COMMAND_ALLOC_CONSTRUCT(1, 3, 2, idiv_command, list, construct_reg64, divider, rax_handle, rdx_handle);
}

void idiv_command::construct_reg64(abstract_register divider, abstract_register rax_handle, abstract_register rdx_handle) {
    type = COMMAND_IDIV;

    set_parameter(0, command_parameter::create_register_64(divider));

    set_read_register(0, rax_handle);
    set_read_register(1, rdx_handle);
    set_read_register(2, divider);
    set_write_register(0, rax_handle);
    set_write_register(1, rdx_handle);
}

void idiv_command::to_bytes(command_encoder* buffer) {

    command_parameter divider = get_parameter(0);

    if(divider.type == PARAMETER_TYPE_REG_64) {
        write_extended_opcode(buffer, 0xF7, 7, divider);

        return;
    }

    return ((asm_command*)this)->to_bytes(buffer);
}

}

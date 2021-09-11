//
// Created by Темыч on 04.05.2021.
//

#include "x86_ret.hpp"
#include "../x86_command_encoder.hpp"

namespace bonk::x86_backend {
ret_command* ret_command::create(command_list* list, abstract_register return_value_handle) {
    if(return_value_handle == -1) {
        X86_COMMAND_ALLOC_CONSTRUCT(0, 0, 0, ret_command, list, construct, return_value_handle);
    } else {
        X86_COMMAND_ALLOC_CONSTRUCT(0, 1, 0, ret_command, list, construct, return_value_handle);
    }
}

void ret_command::construct(abstract_register return_value_handle) {
    type = COMMAND_RET;
    if(return_value_handle != -1) {
        set_read_register(0, return_value_handle);
    }
}

void ret_command::to_bytes(command_encoder* buffer) {
    buffer->push(0xC3);
}
}
//
// Created by Темыч on 06.05.2021.
//

#include "x86_nop.hpp"

namespace bonk::x86_backend {

nop_command* nop_command::create(command_list* list) {
    X86_COMMAND_ALLOC_CONSTRUCT(0, 0, 0, nop_command, list, construct)
}

void nop_command::construct() {
    type = COMMAND_NOP;
}

void nop_command::to_bytes(command_encoder* buffer) {
    buffer->push(0x90);
}

}
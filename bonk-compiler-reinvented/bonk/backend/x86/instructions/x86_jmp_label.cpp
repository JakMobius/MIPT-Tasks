//
// Created by Темыч on 08.05.2021.
//

#include "x86_jmp_label.hpp"

namespace bonk::x86_backend {

jmp_label* jmp_label::create(command_list* list) {
    X86_COMMAND_ALLOC_CONSTRUCT(1, 0, 0, jmp_label, list, construct)
}

void jmp_label::construct() {
    type = COMMAND_JMP_LABEL;
    set_parameter(0, command_parameter::create_imm(list->parent_buffer->labels++));
    jmps_targeting = 0;
}

unsigned long jmp_label::get_index() {
    return get_parameter(0).imm;
}

void jmp_label::to_bytes(command_encoder* buffer) {

}

}
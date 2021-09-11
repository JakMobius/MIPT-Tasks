//
// Created by Темыч on 10.05.2021.
//

#include "x86_colorizer_align_stack.hpp"

namespace bonk::x86_backend {

align_stack_command* align_stack_command::create_before(int stack_entries, command_list* list) {
    X86_COMMAND_ALLOC_CONSTRUCT(1, 0, 0, align_stack_command, list, construct_before, stack_entries);
}

align_stack_command* align_stack_command::create_after(int stack_entries, command_list* list) {
    X86_COMMAND_ALLOC_CONSTRUCT(1, 0, 0, align_stack_command, list, construct_after, stack_entries);
}

void align_stack_command::construct_before(int stack_entries) {
    type = COMMAND_COLORIZER_ALIGN_STACK_BEFORE;

    set_parameter(0, command_parameter::create_imm(stack_entries));
}

void align_stack_command::construct_after(int stack_entries) {
    type = COMMAND_COLORIZER_ALIGN_STACK_AFTER;

    set_parameter(0, command_parameter::create_imm(stack_entries));
}

int align_stack_command::get_stack_entries() {
    return get_parameter(0).imm;
}

}
//
// Created by Темыч on 09.05.2021.
//

#include "x86_colorizer_locate_reg.hpp"

namespace bonk::x86_backend {

locate_reg_command* locate_reg_command::create_reg64_reg64(abstract_register reg, e_machine_register loc, command_list* list) {
    X86_COMMAND_ALLOC_CONSTRUCT(2, 1, 1, locate_reg_command, list, construct_reg64_reg64, reg, loc)
}

void locate_reg_command::construct_reg64_reg64(abstract_register reg, e_machine_register loc) {
    type = COMMAND_COLORIZER_LOCATE_REG_REG;

    set_read_register(0, reg);
    set_write_register(0, reg);
    set_parameter(0, command_parameter::create_register_64(reg));
    set_parameter(1, command_parameter::create_register_64(list->parent_buffer->descriptors.next_constrained_register(loc, list)));
}

locate_reg_command* locate_reg_command::create_reg64_stack(abstract_register reg, int stack_position, command_list* list) {
    X86_COMMAND_ALLOC_CONSTRUCT(2, 1, 1, locate_reg_command, list, create_reg64_stack, reg, stack_position)
}

void locate_reg_command::create_reg64_stack(abstract_register reg, int stack_position) {
    type = COMMAND_COLORIZER_LOCATE_REG_STACK;

    set_read_register(0, reg);
    set_write_register(0, reg);
    set_parameter(0, command_parameter::create_register_64(reg));
    set_parameter(1, command_parameter::create_imm(stack_position));
}

e_machine_register locate_reg_command::get_reg_location() {
    assert(type == COMMAND_COLORIZER_LOCATE_REG_REG);
    return list->parent_buffer->descriptors.get_descriptor(get_parameter(1).reg)->last_register_location;
}

int locate_reg_command::get_stack_location() {
    assert(type == COMMAND_COLORIZER_LOCATE_REG_STACK);
    return (int)get_parameter(1).imm;
}

}
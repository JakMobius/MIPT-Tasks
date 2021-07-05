//
// Created by Темыч on 04.05.2021.
//

#include "x86_colorizer_scope_pop.hpp"

namespace bonk::x86_backend {

scope_pop_command* scope_pop_command::create(int scopes_to_pop, command_list* list) {
    X86_COMMAND_ALLOC_CONSTRUCT(1, 0, 0, scope_pop_command, list, construct, scopes_to_pop);
}

// Passing read_registers and write_registers as parameters here
// to not recalculate these arrays

void scope_pop_command::construct(int scopes_to_pop) {
    type = COMMAND_COLORIZER_SCOPE_POP;
    set_parameter(0, command_parameter::create_imm(scopes_to_pop));
}

}
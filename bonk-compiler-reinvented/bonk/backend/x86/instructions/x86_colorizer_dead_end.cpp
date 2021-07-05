//
// Created by Темыч on 04.05.2021.
//

#include "x86_colorizer_dead_end.hpp"

namespace bonk::x86_backend {

scope_dead_end_command* scope_dead_end_command::create(command_list* list) {
    X86_COMMAND_ALLOC_CONSTRUCT(0, 0, 0, scope_dead_end_command, list, construct);
}

void scope_dead_end_command::construct() {
    type = COMMAND_COLORIZER_SCOPE_DEAD_END;
}

}
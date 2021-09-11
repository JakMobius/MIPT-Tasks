//
// Created by Темыч on 03.05.2021.
//

#include "x86_backend_context_state.hpp"

namespace bonk::x86_backend {

bool backend_context_state::construct(command_list* command_list) {
    current_command_list = command_list;
    return register_stack.construct(command_list);
}

void backend_context_state::destruct() {
    register_stack.destruct();
}

}
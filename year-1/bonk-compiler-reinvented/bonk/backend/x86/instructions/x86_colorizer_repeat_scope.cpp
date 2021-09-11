//
// Created by Темыч on 07.05.2021.
//

#include "x86_colorizer_repeat_scope.hpp"

namespace bonk::x86_backend {

scope_repeat_command* scope_repeat_command::create(command_list* list) {
    X86_COMMAND_ALLOC_CONSTRUCT(0, 0, 0, scope_repeat_command, list, construct);
}


void scope_repeat_command::construct() {
    type = COMMAND_COLORIZER_REPEAT_SCOPE;
}

}
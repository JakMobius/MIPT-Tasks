//
// Created by Темыч on 07.05.2021.
//

#ifndef BONK_COMPILER_REINVENTED_X86_COLORIZER_REPEAT_SCOPE_HPP
#define BONK_COMPILER_REINVENTED_X86_COLORIZER_REPEAT_SCOPE_HPP

#include "../x86_command_buffer.hpp"

namespace bonk::x86_backend {

struct scope_repeat_command: asm_command {

    static scope_repeat_command* create(command_list* list);
    void construct();
};

}

#endif //BONK_COMPILER_REINVENTED_X86_COLORIZER_REPEAT_SCOPE_HPP

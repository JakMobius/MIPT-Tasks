//
// Created by Темыч on 04.05.2021.
//

#ifndef BONK_COMPILER_REINVENTED_X86_COLORIZER_DEAD_END_HPP
#define BONK_COMPILER_REINVENTED_X86_COLORIZER_DEAD_END_HPP

#include "../x86_command_buffer.hpp"

namespace bonk::x86_backend {

struct scope_dead_end_command: asm_command {

    static scope_dead_end_command* create(command_list* list);
    void construct();
};

}

#endif //BONK_COMPILER_REINVENTED_X86_COLORIZER_DEAD_END_HPP

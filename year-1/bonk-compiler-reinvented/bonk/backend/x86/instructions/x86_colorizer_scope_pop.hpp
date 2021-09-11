//
// Created by Темыч on 04.05.2021.
//

#ifndef BONK_COMPILER_REINVENTED_X86_COLORIZER_SCOPE_POP_HPP
#define BONK_COMPILER_REINVENTED_X86_COLORIZER_SCOPE_POP_HPP

#include "../x86_command_buffer.hpp"

namespace bonk::x86_backend {

struct scope_pop_command: asm_command {

    static scope_pop_command* create(int scopes_to_pop, command_list* list);
    void construct(int scopes_to_pop);
};

}

#endif //BONK_COMPILER_REINVENTED_X86_COLORIZER_SCOPE_POP_HPP

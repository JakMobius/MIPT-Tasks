//
// Created by Темыч on 10.05.2021.
//

#ifndef BONK_COMPILER_REINVENTED_X86_COLORIZER_ALIGN_STACK_HPP
#define BONK_COMPILER_REINVENTED_X86_COLORIZER_ALIGN_STACK_HPP

#include "../x86_command_buffer.hpp"

namespace bonk::x86_backend {

struct align_stack_command: asm_command {

    static align_stack_command* create_before(int stack_entries, command_list* list);
    void construct_before(int stack_entries);

    static align_stack_command* create_after(int stack_entries, command_list* list);
    void construct_after(int stack_entries);

    int get_stack_entries();
};

}

#endif //BONK_COMPILER_REINVENTED_X86_COLORIZER_ALIGN_STACK_HPP

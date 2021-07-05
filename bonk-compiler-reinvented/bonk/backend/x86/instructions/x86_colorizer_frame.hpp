//
// Created by Темыч on 08.05.2021.
//

#ifndef BONK_COMPILER_REINVENTED_X86_COLORIZER_FRAME_HPP
#define BONK_COMPILER_REINVENTED_X86_COLORIZER_FRAME_HPP

#include "x86_command.hpp"

namespace bonk::x86_backend {

struct frame_create_command : asm_command {
    static frame_create_command* create(command_list* list);
    void construct();
};

struct frame_destroy_command : asm_command {
    static frame_destroy_command* create(command_list* list);
    void construct();
};

}

#endif //BONK_COMPILER_REINVENTED_X86_COLORIZER_FRAME_HPP

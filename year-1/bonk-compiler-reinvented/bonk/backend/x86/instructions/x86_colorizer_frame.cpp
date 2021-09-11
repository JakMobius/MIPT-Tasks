//
// Created by Темыч on 08.05.2021.
//

#include "x86_colorizer_frame.hpp"

namespace bonk::x86_backend {

frame_create_command* frame_create_command::create(command_list* list) {
    X86_COMMAND_ALLOC_CONSTRUCT(0, 0, 0, frame_create_command, list, construct)
}

void frame_create_command::construct() {
    type = COMMAND_COLORIZER_FRAME_CREATE;
}

frame_destroy_command* frame_destroy_command::create(command_list* list) {
    X86_COMMAND_ALLOC_CONSTRUCT(0, 0, 0, frame_destroy_command, list, construct)
}

void frame_destroy_command::construct() {
    type = COMMAND_COLORIZER_FRAME_DESTROY;
}

}
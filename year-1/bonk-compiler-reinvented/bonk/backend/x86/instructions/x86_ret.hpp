//
// Created by Темыч on 04.05.2021.
//

#ifndef BONK_COMPILER_REINVENTED_X86_RET_HPP
#define BONK_COMPILER_REINVENTED_X86_RET_HPP

#include "x86_command.hpp"
#include "../x86_abstract_register.hpp"
#include "../../../../utils/linear_allocator.hpp"
#include "../x86_command_encoder.hpp"

namespace bonk::x86_backend {

struct ret_command : asm_command {
    static ret_command* create(command_list* list, abstract_register return_value_handle = -1);
    void construct(abstract_register return_value_handle);
    void to_bytes(command_encoder* buffer);
};

}

#endif //BONK_COMPILER_REINVENTED_X86_RET_HPP

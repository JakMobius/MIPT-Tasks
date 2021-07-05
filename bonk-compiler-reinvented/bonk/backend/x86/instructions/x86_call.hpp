//
// Created by Темыч on 08.05.2021.
//

#ifndef BONK_COMPILER_REINVENTED_X86_CALL_HPP
#define BONK_COMPILER_REINVENTED_X86_CALL_HPP

#include "x86_command.hpp"
#include "../x86_backend_context.hpp"

namespace bonk::x86_backend {

struct call_command : asm_command {
    static call_command* create(command_parameter_symbol symbol, abstract_register* parameters, int amount, command_list* list);
    void construct(command_parameter_symbol symbol, abstract_register* parameters, int amount);

    void to_bytes(command_encoder* buffer);
};

}

#endif //BONK_COMPILER_REINVENTED_X86_CALL_HPP

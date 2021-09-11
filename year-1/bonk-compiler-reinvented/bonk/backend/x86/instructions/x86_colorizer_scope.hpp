//
// Created by Темыч on 30.04.2021.
//

#ifndef BONK_COMPILER_REINVENTED_X86_COLORIZER_SCOPE_HPP
#define BONK_COMPILER_REINVENTED_X86_COLORIZER_SCOPE_HPP

#include "x86_command.hpp"

namespace bonk::x86_backend {

struct scope_command: asm_command {

    command_list* commands;

    static scope_command* create(command_list* nested_list, command_list* list);
    void construct(command_list* list, avl_tree<abstract_register>* read_registers, avl_tree<abstract_register>* write_registers);

    void dump(FILE* file, int depth);
};

}

#endif //BONK_COMPILER_REINVENTED_X86_COLORIZER_SCOPE_HPP

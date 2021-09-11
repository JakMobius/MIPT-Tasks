//
// Created by Темыч on 06.05.2021.
//

#ifndef BONK_COMPILER_REINVENTED_X86_TEST_HPP
#define BONK_COMPILER_REINVENTED_X86_TEST_HPP

#include "../x86_abstract_register.hpp"
#include "../../../../utils/linear_allocator.hpp"
#include "x86_command.hpp"

namespace bonk::x86_backend {

struct test_command : asm_command {
    static test_command* create_reg64_reg64(abstract_register target, abstract_register source, command_list* list);
    void construct_reg64_reg64(abstract_register target, abstract_register source);

    static test_command* create_reg8_reg8(abstract_register target, abstract_register source, command_list* list);
    void construct_reg8_reg8(abstract_register target, abstract_register source);

    void to_bytes(command_encoder* buffer);
};

}

#endif //BONK_COMPILER_REINVENTED_X86_TEST_HPP

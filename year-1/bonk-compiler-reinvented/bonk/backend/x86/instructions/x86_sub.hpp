
#ifndef BONK_COMPILER_REINVENTED_X86_SUB_HPP
#define BONK_COMPILER_REINVENTED_X86_SUB_HPP

#include "../x86_abstract_register.hpp"
#include "../../../../utils/linear_allocator.hpp"
#include "x86_command.hpp"

namespace bonk::x86_backend {

struct sub_command : asm_command {
    static sub_command* create_reg64_reg64(abstract_register target, abstract_register source, command_list* list);
    void construct_reg64_reg64(abstract_register target, abstract_register source);

    static sub_command* create_reg64_imm32(abstract_register reg64, uint32_t imm32, command_list* list);
    void construct_reg64_imm32(abstract_register reg64, uint32_t imm32);

    void to_bytes(command_encoder* buffer);
};

}

#endif //BONK_COMPILER_REINVENTED_X86_SUB_HPP

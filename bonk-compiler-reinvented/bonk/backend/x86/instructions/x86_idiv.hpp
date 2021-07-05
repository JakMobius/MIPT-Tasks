//
// Created by Темыч on 25.04.2021.
//

#ifndef BONK_COMPILER_REINVENTED_X86_IDIV_HPP
#define BONK_COMPILER_REINVENTED_X86_IDIV_HPP

#include "x86_command.hpp"
#include "../x86_abstract_register.hpp"
#include "../../../../utils/linear_allocator.hpp"

namespace bonk::x86_backend {

struct idiv_command : asm_command {
    static idiv_command* create_reg64(abstract_register divider, abstract_register rax_handle, abstract_register rdx_handle, command_list* list);
    void construct_reg64(abstract_register divider, abstract_register rax_handle, abstract_register rdx_handle);
    void to_bytes(command_encoder* buffer);
};

}

#endif //BONK_COMPILER_REINVENTED_X86_IDIV_HPP

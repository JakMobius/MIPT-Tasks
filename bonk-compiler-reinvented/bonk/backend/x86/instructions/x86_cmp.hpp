//
// Created by Темыч on 06.05.2021.
//

#include "../x86_abstract_register.hpp"
#include "../../../../utils/linear_allocator.hpp"
#include "x86_command.hpp"

namespace bonk::x86_backend {

struct cmp_command : asm_command {
    static cmp_command* create_reg64_reg64(abstract_register target, abstract_register source, command_list* list);
    void construct_reg64_reg64(abstract_register target, abstract_register source);

    static cmp_command* create_reg8_reg8(abstract_register target, abstract_register source, command_list* list);
    void construct_reg8_reg8(abstract_register target, abstract_register source);

    void to_bytes(command_encoder* buffer);
};

}
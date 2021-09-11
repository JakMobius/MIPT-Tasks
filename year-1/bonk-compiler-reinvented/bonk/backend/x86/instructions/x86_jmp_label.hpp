//
// Created by Темыч on 08.05.2021.
//

#ifndef BONK_COMPILER_REINVENTED_X86_JMP_LABEL_HPP
#define BONK_COMPILER_REINVENTED_X86_JMP_LABEL_HPP

#include "../x86_abstract_register.hpp"
#include "../../../../utils/linear_allocator.hpp"
#include "x86_command.hpp"
#include "../x86_command_encoder.hpp"

namespace bonk::x86_backend {

struct jmp_label : asm_command {

    unsigned long jmps_targeting;

    static jmp_label* create(command_list* list);

    void construct();

    void to_bytes(command_encoder* buffer);

    unsigned long get_index();
};

}

#endif //BONK_COMPILER_REINVENTED_X86_JMP_LABEL_HPP

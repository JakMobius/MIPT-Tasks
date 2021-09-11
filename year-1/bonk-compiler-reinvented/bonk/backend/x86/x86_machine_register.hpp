//
// Created by Темыч on 29.04.2021.
//

#ifndef BONK_COMPILER_REINVENTED_X86_MACHINE_REGISTER_HPP
#define BONK_COMPILER_REINVENTED_X86_MACHINE_REGISTER_HPP

namespace bonk::x86_backend {

enum e_machine_register: unsigned char {
    rax,
    rcx,
    rdx,
    rbx,
    rsp,
    rbp,
    rsi,
    rdi,
    r8,
    r9,
    r10,
    r11,
    r12,
    r13,
    r14,
    r15,
    rinvalid
};

}

#endif //BONK_COMPILER_REINVENTED_X86_MACHINE_REGISTER_HPP

//
// Created by Темыч on 04.05.2021.
//

#ifndef BONK_COMPILER_REINVENTED_X86_REGISTER_USAGE_HPP
#define BONK_COMPILER_REINVENTED_X86_REGISTER_USAGE_HPP

namespace bonk::x86_backend {

struct abstract_register_usage {
    long instruction_index;
    bool is_write;

    bool operator<(const abstract_register_usage& other) const;
};

}

#endif //BONK_COMPILER_REINVENTED_X86_REGISTER_USAGE_HPP

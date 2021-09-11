//
// Created by Темыч on 23.04.2021.
//

#ifndef BONK_COMPILER_REINVENTED_X86_LINKER_HPP
#define BONK_COMPILER_REINVENTED_X86_LINKER_HPP

namespace bonk::x86_backend {

struct linker_block;
struct linker_constant;

}

#include "../../../utils/dynamic_array.cpp"

namespace bonk::x86_backend {

struct linker_block {
    unsigned long long offset;

    dynamic_array<linker_block*> nested_blocks;
    dynamic_array<linker_constant*> constants;
};

struct linker_constant {
    const char* constant_name;
    unsigned long long offset;
};

}

#endif //BONK_COMPILER_REINVENTED_X86_LINKER_HPP

//
// Created by Темыч on 29.04.2021.
//

#ifndef BONK_COMPILER_REINVENTED_X86_FINAL_OPTIMIZER_HPP
#define BONK_COMPILER_REINVENTED_X86_FINAL_OPTIMIZER_HPP

#include "x86_command_buffer.hpp"
#include "../../../utils/hashmap.hpp"
#include "colorizer/x86_colorizer.hpp"

namespace bonk::x86_backend {

struct final_optimizer {
    command_buffer* buffer;
    hash_table<jmp_label*, jmp_label*> label_map;

    static void optimize(command_buffer* buffer);

    bool construct(command_buffer* buffer);
    void destruct();

    void optimize_mov_zeroes();
    void remove_useless_movs();
    void remove_useless_labels();
    bool remove_double_jmps();
    bool remove_dead_ends();

    void apply_label_map();
};

}

#endif //BONK_COMPILER_REINVENTED_X86_FINAL_OPTIMIZER_HPP

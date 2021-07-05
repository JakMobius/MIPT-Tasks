//
// Created by Темыч on 04.05.2021.
//

#ifndef BONK_COMPILER_REINVENTED_X86_COLORIZER_STATE_HPP
#define BONK_COMPILER_REINVENTED_X86_COLORIZER_STATE_HPP

#include "../x86_abstract_register.hpp"
#include "../../../../utils/bin_heap.hpp"
#include "../../../../utils/avl_tree.hpp"
#include "../../../../utils/hashmap.hpp"
#include "x86_register_usage.hpp"
#include "../instructions/x86_command.hpp"

namespace bonk::x86_backend {

struct register_colorize_context_state;

}

namespace bonk::x86_backend {

struct register_colorize_context_state {

    hash_table<abstract_register, bin_heap<abstract_register_usage>*> usage_heap_array;

    // Relocation table
    hash_table<abstract_register, abstract_register_descriptor> relocation_info;
    command_list* source;

    bool construct(command_list* list, register_colorize_context_state* state);
    void destruct();

    bool create_usage_heap_array();
    void add_register_usage(abstract_register, abstract_register_usage usage);

    void duplicate_usages();
};

}

#endif //BONK_COMPILER_REINVENTED_X86_COLORIZER_STATE_HPP

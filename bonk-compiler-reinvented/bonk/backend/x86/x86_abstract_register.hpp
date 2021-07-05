//
// Created by Темыч on 25.04.2021.
//

#ifndef BONK_COMPILER_REINVENTED_X86_ABSTRACT_REGISTER_HPP
#define BONK_COMPILER_REINVENTED_X86_ABSTRACT_REGISTER_HPP

namespace bonk::x86_backend {

typedef long long abstract_register;
struct abstract_register_descriptor;
struct command_list;

}

#include "x86_backend.hpp"
#include "x86_machine_register.hpp"

namespace bonk::x86_backend {

struct abstract_register_descriptor {

    // As symbol-located register may not be stored in
    // stack frame, it become possible to unite these fields
    // to reduce memory consumption
    union {
        /// Current or last position in stack frame of this register
        int last_memory_position;
        /// Symbol position of this register
        uint32_t symbol_position;
    };

    /// Current or last machine register this register used to be stored in
    e_machine_register last_register_location;

    /// Indicates if this register should not be located
    /// in any register except one specified in last_register_location
    /// field
    bool has_register_constraint;

    /// Indicates if this register has location in data section
    /// specified by symbol_position field
    bool has_symbol_position;

    /// Indicates it this register is now located in real machine register
    /// specified by last_register_location field
    bool located_in_register;

    /// Indicates if this register is now stored in stack frame.
    /// the offset is stored in last_memory_position
    bool located_in_memory;

    /// Indicates if this register is now stored in data section.
    /// The symbol is stored in symbol_position field
    bool located_in_symbol;

    // Registers may only be relocated by their owners
    command_list* owner;
};

}

#endif //BONK_COMPILER_REINVENTED_X86_ABSTRACT_REGISTER_HPP

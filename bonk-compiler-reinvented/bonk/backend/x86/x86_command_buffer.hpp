//
// Created by Темыч on 29.04.2021.
//

#ifndef BONK_COMPILER_REINVENTED_X86_COMMAND_BUFFER_HPP
#define BONK_COMPILER_REINVENTED_X86_COMMAND_BUFFER_HPP

namespace bonk::x86_backend {

struct command_buffer;
struct command_list;

}

#include "instructions/x86_command.hpp"
#include "../../../utils/linear_allocator.hpp"
#include "../../../utils/avl_tree.hpp"
#include "x86_register_descriptor_list.hpp"
#include "x86_command_encoder.hpp"
#include "../../target/macho/macho.hpp"

namespace bonk::x86_backend {

struct command_list : mlist<asm_command*> {
    command_buffer* parent_buffer;

    static command_list* create(command_buffer* parent_buffer);
    bool construct(command_buffer* parent_buffer);

    bool append_read_register(avl_tree<abstract_register>* tree);
    bool append_write_register(avl_tree<abstract_register>* tree);

    void dump(FILE* file, int depth);
};

struct command_buffer {
    macho::macho_file* file;
    command_list* root_list;
    linear_allocator allocator;
    register_descriptor_list descriptors;
    dynamic_array<command_list*> lists;
    unsigned long labels;

    // Creates completely new independent command parent_buffer
    static command_buffer* create(macho::macho_file* file = nullptr);
    bool construct(macho::macho_file* file = nullptr);

    // Creates command parent_buffer which will use the same register namespace as
    // another command parent_buffer
    static command_buffer* create(register_descriptor_list* descriptor_list, macho::macho_file* file = nullptr);
    bool construct(register_descriptor_list* descriptor_list, macho::macho_file* file = nullptr);

    command_list* next_command_list();

    void destruct();
    void release();

    void dump(FILE* file, int depth = 0);
    void write_block_to_object_file(const char* block_name, macho::macho_file* target);

    command_encoder* to_bytes();
};

}

#endif //BONK_COMPILER_REINVENTED_X86_COMMAND_BUFFER_HPP

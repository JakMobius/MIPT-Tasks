
#ifndef BONK_COMPILER_REINVENTED_X86_REGISTER_DESCRIPTOR_LIST_HPP
#define BONK_COMPILER_REINVENTED_X86_REGISTER_DESCRIPTOR_LIST_HPP

#include "x86_abstract_register.hpp"

namespace bonk::x86_backend {

struct register_descriptor_list {
    dynamic_array<abstract_register_descriptor> array;
    register_descriptor_list* parent_descriptor_list;
    unsigned long offset;

    static register_descriptor_list* create(register_descriptor_list* other);
    bool construct(register_descriptor_list* other);

    static register_descriptor_list* create();
    bool construct();

    void destruct();
    void release();

    unsigned long size();

    abstract_register next_constrained_register(e_machine_register reg, command_list* owner);
    abstract_register next_register(command_list* owner);
    abstract_register machine_register(e_machine_register reg);

    abstract_register_descriptor* get_descriptor(abstract_register i);
};

}

#endif //BONK_COMPILER_REINVENTED_X86_REGISTER_DESCRIPTOR_LIST_HPP

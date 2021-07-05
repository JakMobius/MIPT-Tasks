//
// Created by Темыч on 23.04.2021.
//

#ifndef BONK_COMPILER_REINVENTED_X86_REG_STACK_HPP
#define BONK_COMPILER_REINVENTED_X86_REG_STACK_HPP

namespace bonk::x86_backend {

struct reg_stack;
struct command_list;
struct reg_stack_entry;

}

#include "x86_abstract_register.hpp"
#include "../../../utils/default_alloc_construct.hpp"
#include "instructions/x86_cmp.hpp"
#include "instructions/x86_conditional_instructions.hpp"
#include "instructions/x86_logical_instructions.hpp"
#include "instructions/x86_test.hpp"
#include <cstdlib>

namespace bonk::x86_backend {

struct reg_stack_entry {
    abstract_register reg;
    bool is_logical;
};

struct reg_stack {

    dynamic_array<reg_stack_entry> stack;

    command_list* list;

    static reg_stack* create(command_list* list);
    bool construct(command_list* list);

    void destruct();
    void release();

    void push_imm64(uint64_t value);
    void push_reg64(abstract_register reg);
    void pop(abstract_register reg);
    void add();
    void sub();
    void mul();
    void div();

    reg_stack_entry get_head_register();

    abstract_register get_head_register_number();
    abstract_register get_head_register_logical();

    bool push_next_register(bool is_logical);
    void decrease_stack_size();

    void logical_and();

    void logical_or();

    void set_head_type(bool is_logical);

    void equals();

    void less_than();

    void less_or_equal_than();

    void greater_than();

    void greater_or_equal_than();

    void not_equal();

    void convert_logical_to_number(abstract_register reg);

    void convert_number_to_logical(abstract_register reg);

    void compare();

    void test();

    abstract_register push_placeholder(bool is_logical);

    void pop_logical(abstract_register reg);

    void write_head(abstract_register reg);
};

}

#endif //BONK_COMPILER_REINVENTED_X86_REG_STACK_HPP

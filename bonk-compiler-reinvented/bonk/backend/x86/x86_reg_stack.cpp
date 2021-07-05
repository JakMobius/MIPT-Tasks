//
// Created by Темыч on 23.04.2021.
//

#include "x86_reg_stack.hpp"
#include "instructions/x86_xor.hpp"
#include "instructions/x86_mov.hpp"
#include "instructions/x86_idiv.hpp"
#include "instructions/x86_imul.hpp"
#include "instructions/x86_sub.hpp"
#include "instructions/x86_add.hpp"
#include "x86_register_extensions.hpp"

namespace bonk::x86_backend {

reg_stack* reg_stack::create(command_list* list) {
    CHECK_ALLOC_CONSTRUCT(reg_stack, list);
}

bool reg_stack::construct(command_list* command_list) {
    list = command_list;
    if(stack.construct(16)) return false;
    return true;
}

void reg_stack::destruct() {

}

void reg_stack::release() {
    destruct();
    free(this);
}

void reg_stack::push_imm64(uint64_t value) {
    push_next_register(false);
    list->insert_tail(mov_command::create_reg64_imm64(get_head_register().reg, value, list));
}

abstract_register reg_stack::push_placeholder(bool is_logical) {
    push_next_register(is_logical);
    return get_head_register().reg;
}

void reg_stack::push_reg64(abstract_register reg) {
    push_next_register(false);
    list->insert_tail(mov_command::create_reg64_reg64(get_head_register().reg, reg, list));
}

void reg_stack::logical_and() {
    abstract_register source = get_head_register_logical();
    decrease_stack_size();
    abstract_register destination = get_head_register_logical();

    list->insert_tail(and_command::create_reg8_reg8(destination, source, list));

    set_head_type(true);
}

void reg_stack::logical_or() {
    abstract_register source = get_head_register_logical();
    decrease_stack_size();
    abstract_register destination = get_head_register_logical();

    list->insert_tail(or_command::create_reg8_reg8(destination, source, list));

    set_head_type(true);
}

void reg_stack::test() {
    reg_stack_entry entry = get_head_register();

    if(entry.is_logical) {
        list->insert_tail(test_command::create_reg8_reg8(entry.reg, entry.reg, list));
    } else {
        list->insert_tail(test_command::create_reg64_reg64(entry.reg, entry.reg, list));
    }

    decrease_stack_size();
}

void reg_stack::compare() {
    reg_stack_entry right = get_head_register();
    decrease_stack_size();
    reg_stack_entry left = get_head_register();
    decrease_stack_size();

    if(right.is_logical != left.is_logical) {
        if(right.is_logical) convert_logical_to_number(right.reg);
        else convert_logical_to_number(left.reg);
    }
    if(right.is_logical) {
        list->insert_tail(cmp_command::create_reg8_reg8(left.reg, right.reg, list));
    } else {
        list->insert_tail(cmp_command::create_reg64_reg64(left.reg, right.reg, list));
    }
}

void reg_stack::equals() {
    compare();
    push_next_register(true);
    list->insert_tail(sete_command::create_reg8(get_head_register().reg, list));
}

void reg_stack::less_than() {
    compare();
    push_next_register(true);
    list->insert_tail(setl_command::create_reg8(get_head_register().reg, list));
}

void reg_stack::less_or_equal_than() {
    compare();
    push_next_register(true);
    list->insert_tail(setng_command::create_reg8(get_head_register().reg, list));}

void reg_stack::greater_than() {
    compare();
    push_next_register(true);
    list->insert_tail(setg_command::create_reg8(get_head_register().reg, list));
}

void reg_stack::greater_or_equal_than() {
    compare();
    push_next_register(true);
    list->insert_tail(setge_command::create_reg8(get_head_register().reg, list));
}

void reg_stack::not_equal() {
    compare();
    push_next_register(true);
    list->insert_tail(setne_command::create_reg8(get_head_register().reg, list));
}

void reg_stack::add() {
    abstract_register source = get_head_register_number();
    decrease_stack_size();
    abstract_register destination = get_head_register_number();

    list->insert_tail(add_command::create_reg64_reg64(destination, source, list));

    set_head_type(false);
}

void reg_stack::sub() {
    abstract_register source = get_head_register_number();
    decrease_stack_size();
    abstract_register destination = get_head_register_number();
    list->insert_tail(sub_command::create_reg64_reg64(destination, source, list));

    set_head_type(false);
}

void reg_stack::mul() {
    abstract_register source = get_head_register_number();
    decrease_stack_size();
    abstract_register destination = get_head_register_number();
    list->insert_tail(imul_command::create_reg64_reg64(destination, source, list));

    set_head_type(false);
}

void reg_stack::div() {
    abstract_register source = get_head_register_number();
    decrease_stack_size();
    abstract_register destination = get_head_register_number();

    abstract_register rdx_handle = list->parent_buffer->descriptors.next_constrained_register(rdx, list);
    abstract_register rax_handle = list->parent_buffer->descriptors.next_constrained_register(rax, list);

    list->insert_tail(xor_command::create_reg64_reg64(rdx_handle, rdx_handle, list));
    list->insert_tail(mov_command::create_reg64_reg64(rax_handle, destination, list));
    list->insert_tail(idiv_command::create_reg64(source, rax_handle, rdx_handle, list));
    list->insert_tail(mov_command::create_reg64_reg64(destination, rax_handle, list));

    set_head_type(false);
}

reg_stack_entry reg_stack::get_head_register() {
    return stack.get(stack.size - 1);
}

void reg_stack::set_head_type(bool is_logical) {
    stack.point(stack.size - 1)->is_logical = is_logical;
}

bool reg_stack::push_next_register(bool is_logical) {
    abstract_register reg = list->parent_buffer->descriptors.next_register(list);

    if(stack.push({reg, is_logical})) return false;
    return true;
}

void reg_stack::decrease_stack_size() {
    stack.pop();
}

void reg_stack::write_head(abstract_register reg) {
    list->insert_tail(mov_command::create_reg64_reg64(reg, get_head_register_number(), list));
}

void reg_stack::pop(abstract_register reg) {
    write_head(reg);
    decrease_stack_size();
}

void reg_stack::convert_logical_to_number(abstract_register reg) {
    list->insert_tail(movzx_command::create_reg64_reg8(reg, reg, list));
}

void reg_stack::convert_number_to_logical(abstract_register reg) {
    list->insert_tail(test_command::create_reg64_reg64(reg, reg, list));
    list->insert_tail(setne_command::create_reg8(reg, list));
}

abstract_register reg_stack::get_head_register_number() {
    auto head = get_head_register();
    if(head.is_logical) convert_logical_to_number(head.reg);
    return head.reg;
}

abstract_register reg_stack::get_head_register_logical() {
    auto head = get_head_register();
    if(!head.is_logical) convert_number_to_logical(head.reg);
    return head.reg;
}

}
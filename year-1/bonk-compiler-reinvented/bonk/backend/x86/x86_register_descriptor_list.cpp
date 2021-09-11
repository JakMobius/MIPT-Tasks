//
// Created by Темыч on 29.04.2021.
//

#include "x86_register_descriptor_list.hpp"

namespace bonk::x86_backend {


register_descriptor_list* register_descriptor_list::create() {
    CHECK_ALLOC_CONSTRUCT(register_descriptor_list);
}

register_descriptor_list* register_descriptor_list::create(register_descriptor_list* other) {
    CHECK_ALLOC_CONSTRUCT(register_descriptor_list, other);
}

bool register_descriptor_list::construct(register_descriptor_list* parent) {
    if(array.construct(16)) return false;
    parent_descriptor_list = parent;
    offset = parent_descriptor_list->offset + parent_descriptor_list->array.size;

    return true;
}

bool register_descriptor_list::construct() {
    if(array.construct(16)) return false;
    parent_descriptor_list = nullptr;

    for(int i = 0; i < 16; i++) {
        array.push({
            .last_register_location = e_machine_register(i),
            .has_register_constraint = true,
            .located_in_register = false,
            .located_in_memory = false,
            .owner = nullptr
        });
    }

    return true;
}

void register_descriptor_list::destruct() {
    array.destruct();
}

void register_descriptor_list::release() {
    destruct();
    free(this);
}

abstract_register register_descriptor_list::next_register(command_list* owner) {
    array.push({
        .last_register_location = rinvalid,
        .has_register_constraint = false,
        .located_in_register = false,
        .located_in_memory = false,
        .owner = owner
    });
    return array.size - 1 + offset;
}

abstract_register register_descriptor_list::next_constrained_register(e_machine_register reg, command_list* owner) {
    array.push({
       .last_register_location = reg,
       .has_register_constraint = true,
       .located_in_register = false,
       .located_in_memory = false,
       .owner = owner
    });
    return array.size - 1 + offset;
}

abstract_register_descriptor* register_descriptor_list::get_descriptor(abstract_register i) {
    assert(i >= 0 && i < size());
    if(i < offset) return parent_descriptor_list->get_descriptor(i);
    return array.point(i - offset);
}

abstract_register register_descriptor_list::machine_register(e_machine_register reg) {
    //if(parent_descriptor_list) return parent_descriptor_list->machine_register(reg);
    return abstract_register(reg);
}

unsigned long register_descriptor_list::size() {
    return offset + array.size;
}

}
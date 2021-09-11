//
// Created by Темыч on 08.05.2021.
//

#include "x86_call.hpp"

namespace bonk::x86_backend {

call_command* call_command::create(command_parameter_symbol symbol, abstract_register* parameters, int amount, command_list* list) {
    X86_COMMAND_ALLOC_CONSTRUCT(1, amount, 9, call_command, list, construct, symbol, parameters, amount);
}

void call_command::construct(command_parameter_symbol symbol, abstract_register* parameters, int amount) {
    type = COMMAND_CALL;
    assert(symbol.ip_relative);
    set_parameter(0, command_parameter::create_symbol(symbol));

    for(int i = 0; i < amount; i++) {
        set_read_register(i, parameters[i]);
    }

    auto descriptors = &list->parent_buffer->descriptors;

    for(int i = 0; i < SYSTEM_V_CALLER_PRESERVED_REGISTERS_COUNT; i++) {
        set_write_register(i, descriptors->next_constrained_register(SYSTEM_V_CALLER_PRESERVED_REGISTERS[i], list));
    }
}

void call_command::to_bytes(command_encoder* buffer) {
    buffer->push(0xE8);
    int32_t current_offset = buffer->size;
    for(int i = 0; i < 4; i++) buffer->push(0x00);

    buffer->request_relocation(relocation_request {
        .relocation = get_parameter(0).symbol.symbol,
        .address = current_offset,
        .type = macho::RELOCATION_TYPE_CALL,
        .data_length = 4,
        .pc_rel = true
    });
}

}
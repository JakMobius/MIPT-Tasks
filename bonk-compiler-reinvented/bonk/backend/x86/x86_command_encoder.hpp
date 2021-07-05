//
// Created by Темыч on 06.05.2021.
//

#ifndef BONK_COMPILER_REINVENTED_X86_COMMAND_ENCODER_HPP
#define BONK_COMPILER_REINVENTED_X86_COMMAND_ENCODER_HPP

namespace bonk::x86_backend {

struct emplace_request;
struct command_encoder;
struct jmp_label;

}

#include "../../../utils/dynamic_array.hpp"
#include "../../../utils/default_alloc_construct.hpp"
#include "instructions/x86_command.hpp"
#include "../../target/macho/macho.hpp"

namespace bonk::x86_backend {

struct emplace_request {
    unsigned long offset;
    unsigned long relation;
    char bytes;
    jmp_label* command;
};

struct relocation_request {
    uint32_t relocation;
    int32_t address;
    macho::relocation_type type;
    uint8_t data_length;
    bool pc_rel;
};

struct command_encoder : dynamic_array<char> {
    dynamic_array<emplace_request> emplace_requests;
    dynamic_array<relocation_request> relocation_requests;

    static command_encoder* create();
    bool construct();

    void destruct();
    void release();

    void request_emplace(emplace_request request);

    void do_emplacements();

    void request_relocation(relocation_request request);
};

}

#endif //BONK_COMPILER_REINVENTED_X86_COMMAND_ENCODER_HPP

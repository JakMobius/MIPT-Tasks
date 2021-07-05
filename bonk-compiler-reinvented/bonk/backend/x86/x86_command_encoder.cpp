//
// Created by Темыч on 06.05.2021.
//

#include "x86_command_encoder.hpp"
#include "instructions/x86_jmp_label.hpp"

namespace bonk::x86_backend {


command_encoder* command_encoder::create() {
    CHECK_ALLOC_CONSTRUCT(command_encoder)
}

bool command_encoder::construct() {
    if(((dynamic_array<char*>*)this)->construct()) return false;
    if(emplace_requests.construct()) return false;
    if(relocation_requests.construct()) return false;
    return true;
}

void command_encoder::destruct() {
    ((dynamic_array<char*>*)this)->destruct();
    emplace_requests.destruct();
    relocation_requests.destruct();
}

void command_encoder::request_emplace(emplace_request request) {
    emplace_requests.push(request);
}

void command_encoder::release() {
    destruct();
    free(this);
}

void command_encoder::do_emplacements() {
    for(int i = 0; i < emplace_requests.size; i++) {
        emplace_request* request = emplace_requests.point(i);
        if(!request->command) continue;
        long value = request->command->offset - request->relation;
        for(int j = 0; j < request->bytes; j++) {
            set(j + request->offset, ((char*)&value)[j]);
        }
    }
}

void command_encoder::request_relocation(relocation_request request) {
    relocation_requests.push(request);
}

}
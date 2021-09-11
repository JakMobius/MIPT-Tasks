
#include "x86_command_buffer.hpp"
#include "instructions/x86_add.hpp"
#include "instructions/x86_sub.hpp"
#include "instructions/x86_imul.hpp"
#include "instructions/x86_idiv.hpp"
#include "instructions/x86_xor.hpp"
#include "instructions/x86_conditional_instructions.hpp"
#include "instructions/x86_colorizer_scope.hpp"
#include "instructions/x86_push.hpp"
#include "instructions/x86_mov.hpp"
#include "instructions/x86_colorizer_dead_end.hpp"
#include "instructions/x86_colorizer_reg_preserve.hpp"
#include "instructions/x86_colorizer_locate_reg.hpp"
#include "instructions/x86_colorizer_align_stack.hpp"

namespace bonk::x86_backend {

command_buffer* command_buffer::create(macho::macho_file* file) {
    CHECK_ALLOC_CONSTRUCT(command_buffer, file);
}

bool command_buffer::construct(macho::macho_file* the_file) {
    file = the_file;
    if(!allocator.construct(4096)) return false;
    if(!descriptors.construct()) return false;
    if(lists.construct(4)) return false;
    root_list = next_command_list();
    if(!root_list) return false;
    return true;
}

command_buffer* command_buffer::create(register_descriptor_list* descriptor_list, macho::macho_file* file) {
    CHECK_ALLOC_CONSTRUCT(command_buffer, descriptor_list, file);
}

bool command_buffer::construct(register_descriptor_list* descriptor_list, macho::macho_file* the_file) {
    file = the_file;
    if(!allocator.construct(4096)) return false;
    if(!descriptors.construct(descriptor_list)) return false;
    if(lists.construct(4)) return false;
    root_list = next_command_list();
    if(!root_list) return false;
    return true;
}

void command_buffer::destruct() {
    for(int i = 0; i < lists.size; i++) {
        lists.get(i)->release();
    }
    lists.destruct();
    allocator.destruct();
}

void command_buffer::release() {
    destruct();
    free(this);
}

void command_buffer::dump(FILE* file, int depth) {
    root_list->dump(file, depth);
}

command_list* command_buffer::next_command_list() {
    auto* next_list = command_list::create(this);
    if(!next_list) return nullptr;
    if(lists.push(next_list)) {
        next_list->release();
        return nullptr;
    }

    return next_list;
}

command_encoder* command_buffer::to_bytes() {
    command_encoder* encoder = command_encoder::create();

    for(auto i = root_list->begin(); i != root_list->end(); root_list->next_iterator(&i)) {
        asm_command* command = root_list->get(i);
        command->offset = encoder->size;
        switch(command->type) {
            /* These cases would assert(false) */
            case COMMAND_COLORIZER_SCOPE_DEAD_END:    ((scope_dead_end_command*)command)->to_bytes(encoder); break;
            case COMMAND_COLORIZER_SCOPE_POP:         ((scope_pop_command*)     command)->to_bytes(encoder); break;
            case COMMAND_COLORIZER_SCOPE:             ((scope_command*)         command)->to_bytes(encoder); break;
            case COMMAND_COLORIZER_REPEAT_SCOPE:      ((scope_repeat_command*)  command)->to_bytes(encoder); break;
            case COMMAND_COLORIZER_REG_PRESERVE:      ((reg_preserve_command*)  command)->to_bytes(encoder); break;
            case COMMAND_COLORIZER_FRAME_CREATE:      ((frame_create_command*)  command)->to_bytes(encoder); break;
            case COMMAND_COLORIZER_FRAME_DESTROY:     ((frame_destroy_command*) command)->to_bytes(encoder); break;
            case COMMAND_COLORIZER_LOCATE_REG_REG:
            case COMMAND_COLORIZER_LOCATE_REG_STACK:  ((locate_reg_command*)    command)->to_bytes(encoder); break;
            case COMMAND_COLORIZER_ALIGN_STACK_BEFORE:
            case COMMAND_COLORIZER_ALIGN_STACK_AFTER: ((align_stack_command*)   command)->to_bytes(encoder); break;
            /* Ignored instructions */
            case COMMAND_JMP_LABEL:                  ((jmp_label*)             command)->to_bytes(encoder); break;
            /* Real assembly instructions */
            case COMMAND_MOV:  ((mov_command*)   command)->to_bytes(encoder); break;
            case COMMAND_ADD:  ((add_command*)   command)->to_bytes(encoder); break;
            case COMMAND_SUB:  ((sub_command*)   command)->to_bytes(encoder); break;
            case COMMAND_IMUL: ((imul_command*)  command)->to_bytes(encoder); break;
            case COMMAND_IDIV: ((idiv_command*)  command)->to_bytes(encoder); break;
            case COMMAND_XOR:  ((xor_command*)   command)->to_bytes(encoder); break;
            case COMMAND_PUSH: ((push_command*)  command)->to_bytes(encoder); break;
            case COMMAND_POP:  ((pop_command*)   command)->to_bytes(encoder); break;
            case COMMAND_JMP:  ((jmp_command*)   command)->to_bytes(encoder); break;
            case COMMAND_JE:   ((je_command*)    command)->to_bytes(encoder); break;
            case COMMAND_JNE:  ((jne_command*)   command)->to_bytes(encoder); break;
            case COMMAND_JG:   ((jg_command*)    command)->to_bytes(encoder); break;
            case COMMAND_JNG:  ((jng_command*)   command)->to_bytes(encoder); break;
            case COMMAND_JL:   ((jl_command*)    command)->to_bytes(encoder); break;
            case COMMAND_JNL:  ((jge_command*)   command)->to_bytes(encoder); break;
            case COMMAND_SETE: ((sete_command*)  command)->to_bytes(encoder); break;
            case COMMAND_SETNE:((setne_command*) command)->to_bytes(encoder); break;
            case COMMAND_SETG: ((setg_command*)  command)->to_bytes(encoder); break;
            case COMMAND_SETNG:((setng_command*) command)->to_bytes(encoder); break;
            case COMMAND_SETL: ((setl_command*)  command)->to_bytes(encoder); break;
            case COMMAND_SETNL:((setge_command*) command)->to_bytes(encoder); break;
            case COMMAND_XCHG: ((xchg_command*)  command)->to_bytes(encoder); break;
            case COMMAND_RET:  ((ret_command*)   command)->to_bytes(encoder); break;
            case COMMAND_AND:  ((and_command*)   command)->to_bytes(encoder); break;
            case COMMAND_OR:   ((or_command*)    command)->to_bytes(encoder); break;
            case COMMAND_TEST: ((test_command*)  command)->to_bytes(encoder); break;
            case COMMAND_MOVZX:((movzx_command*) command)->to_bytes(encoder); break;
            case COMMAND_CMP:  ((cmp_command*)   command)->to_bytes(encoder); break;
            case COMMAND_NOP:  ((nop_command*)   command)->to_bytes(encoder); break;
            case COMMAND_CALL: ((call_command*)  command)->to_bytes(encoder); break;
            default: assert(false);
        }
    }

    return encoder;
}

void command_buffer::write_block_to_object_file(const char* block_name, macho::macho_file* target) {

    auto encoder = to_bytes();
    auto offset = target->section_text.size;

    encoder->do_emplacements();
    for(int i = 0; i < encoder->relocation_requests.size; i++) {
        auto* relocation_request = encoder->relocation_requests.point(i);

        target->add_relocation(
                relocation_request->relocation,
                relocation_request->address + offset,
                relocation_request->pc_rel,
                relocation_request->data_length);
    }

    target->add_internal_symbol(block_name, macho::SYMBOL_SECTION_TEXT, offset);
    target->add_code(encoder);
}

bool command_list::append_read_register(avl_tree<abstract_register>* tree) {
    for(auto i = begin(); i != end(); next_iterator(&i)) {
        asm_command* command = get(i);
        for(int j = 0; j < command->total_read_registers; j++) {
            auto reg = command->get_read_register(j);
            if(parent_buffer->descriptors.get_descriptor(reg)->owner == this) continue;
            if(!tree->insert(reg)) return false;
        }
    }
    return true;
}

bool command_list::append_write_register(avl_tree<abstract_register>* tree) {
    for(auto i = begin(); i != end(); i = next_iterator(&i)) {
        asm_command* command = get(i);
        for(int j = 0; j < command->total_write_registers; j++) {
            auto reg = command->get_write_register(j);
            if(parent_buffer->descriptors.get_descriptor(reg)->owner == this) continue;
            if(!tree->insert(reg)) return false;
        }
    }
    return true;
}

void command_list::dump(FILE* file, int depth) {
    for(auto i = begin(); i != end(); next_iterator(&i)) {
        get(i)->dump(file, depth);
        fputc('\n', file);
    }
}

command_list* command_list::create(command_buffer* parent_buffer) {
    CHECK_ALLOC_CONSTRUCT(command_list, parent_buffer);
}

bool command_list::construct(command_buffer* parent) {
    parent_buffer = parent;
    return ((mlist<asm_command>*)this)->construct(128);
}
}
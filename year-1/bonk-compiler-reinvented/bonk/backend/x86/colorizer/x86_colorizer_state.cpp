//
// Created by Темыч on 04.05.2021.
//

#include "x86_colorizer_state.hpp"
#include "x86_colorizer.hpp"

namespace bonk::x86_backend {

bool register_colorize_context_state::construct(command_list* list, register_colorize_context_state* old_state) {
    relocation_info.construct(4);

    source = list;
    if(!create_usage_heap_array()) return false;

    return true;
}

void register_colorize_context_state::destruct() {
    relocation_info.destruct();
    for(int i = 0; i < usage_heap_array.capacity; i++) {
        auto chain = &usage_heap_array.lists[i];
        for(auto j = chain->begin(); j != chain->end(); chain->next_iterator(&j)) {
            chain->get(j).value->release();
        }
    }
    usage_heap_array.destruct();
}

void register_colorize_context_state::add_register_usage(abstract_register reg, abstract_register_usage usage) {
    if(!usage_heap_array.has(reg)) {
        auto new_usage_heap = bin_heap<abstract_register_usage>::create(4);
        new_usage_heap->push(usage);
        usage_heap_array.insert(reg, new_usage_heap);
    } else {
        usage_heap_array.get(reg)->push(usage);
    }
}

bool register_colorize_context_state::create_usage_heap_array() {
    if(!usage_heap_array.construct(8)) return false;

    int instruction_index = 0;

    for(auto i = source->begin(); i != source->end(); source->next_iterator(&i)) {
        asm_command* command = source->get(i);

        if(command->type == COMMAND_COLORIZER_REPEAT_SCOPE) {
            duplicate_usages();
            // "repeat scope" should be last command in the scope.
            assert(source->next_iterator(i) == source->end());
        }

        for(int j = 0; j < command->total_read_registers; j++) {
            add_register_usage(command->get_read_register(j), {(long)instruction_index, false});
        }

        for(int j = 0; j < command->total_write_registers; j++) {
            add_register_usage(command->get_write_register(j), {(long)instruction_index, true});
        }

        instruction_index++;
    }

    return true;
}

void register_colorize_context_state::duplicate_usages() {
    for(int i = 0; i < usage_heap_array.capacity; i++) {
        auto chain = &usage_heap_array.lists[i];
        for(auto j = chain->begin(); j != chain->end(); chain->next_iterator(&j)) {
            hash_table_chain_entry<abstract_register, bin_heap<abstract_register_usage>*>* entry = nullptr;
            chain->get(j, &entry);
            if(source->parent_buffer->descriptors.get_descriptor(entry->key)->owner != source) {
                entry->value->push({(long) (source->get_size() + 1), false});
            }
        }
    }
}

}
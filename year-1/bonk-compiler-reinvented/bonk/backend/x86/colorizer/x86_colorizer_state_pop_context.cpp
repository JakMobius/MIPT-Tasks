
#include "x86_colorizer_state_pop_context.hpp"
#include "x86_colorizer.hpp"

namespace bonk::x86_backend {

bool colorizer_state_pop_context::construct(register_colorizer* the_colorizer, int the_states_to_pop) {
    colorizer = the_colorizer;
    states_to_pop = the_states_to_pop;

    if(!target_transform.construct(4)) return false;
    COLORIZER_DEBUG("restoring %d states\n", states_to_pop);
    if(!count_transform()) return false;
    return true;
}

void colorizer_state_pop_context::destruct() {
    target_transform.destruct();
}

bool colorizer_state_pop_context::count_transform() {
    auto stack = &colorizer->state_stack;
    int pop = 0;
    for(long long i = stack->size - 1; pop < states_to_pop; i--, pop++) {
        auto state = stack->point(i);
        auto list = state->source;
        auto single_transform = state->relocation_info;

        // Joining transform map to our target transform

        for(int j = 0; j < single_transform.capacity; j++) {
            auto chain = &single_transform.lists[j];
            for(auto k = chain->begin(); k != chain->end(); chain->next_iterator(&k)) {
                hash_table_chain_entry<abstract_register, abstract_register_descriptor>* pair = {};
                chain->get(k, &pair);
                if(target_transform.insert(pair->key, pair->value)) return false;
            }
        }

        // Removing registers we've owned in current state

        for(int j = 0; j < target_transform.capacity; j++) {
            auto chain = &target_transform.lists[j];
            for(auto k = chain->begin(); k != chain->end();) {
                hash_table_chain_entry<abstract_register, abstract_register_descriptor>* pair = {};
                chain->get(k, &pair);
                if(pair->value.owner == list) {
                    auto old = k;
                    chain->next_iterator(&k);
                    chain->remove(old);
                    continue;
                }
                chain->next_iterator(&k);
            }
        }
    }

    for(int j = 0; j < target_transform.capacity; j++) {
        auto chain = &target_transform.lists[j];
        for(auto k = chain->begin(); k != chain->end();) {
            hash_table_chain_entry<abstract_register, abstract_register_descriptor>* pair = {};
            chain->get(k, &pair);
#ifdef COLORIZER_DEBUG_ON
            printf("Should restore _r%lld ", pair->key);
            if(pair->value.located_in_register) printf("to %s, ", ASM_REGISTERS_64[pair->value.last_register_location]);
            if(pair->value.located_in_memory) printf("to memory, ");
            auto descriptor = get_current_descriptor(pair->key);
            if(descriptor.located_in_register) printf("now in %s\n", ASM_REGISTERS_64[descriptor.last_register_location]);
            if(descriptor.located_in_memory) printf("now in memory\n");
#endif
            chain->next_iterator(&k);
        }
    }
    return true;
}

void colorizer_state_pop_context::walk_register_location_graph(bool* is_walked, abstract_register i) {
    auto descriptor = get_current_descriptor(i);
    if(!should_save_register_with_owner(descriptor.owner)) {
        colorizer->release_register_location(i, true);
        return;
    }

    auto old_state = get_target_descriptor(i);

    if(!old_state.located_in_register) {
        colorizer->release_register_location(i, true);
        return;
    }

    e_machine_register current_location = descriptor.last_register_location;
    e_machine_register old_location = old_state.last_register_location;

    if(current_location == old_location) {
        // Everything is already on place
        return;
    }

    if(is_walked[current_location]) {
        // This situation happens when there is a circuit in location graph.
        // i.e, if _r1 and _r2 are used to be stored in rax and rbx, but
        // after some block their registers switched places (so they are
        // now stored in rbx, rax.
        // This can be resolved by exchanging places of last two
        // registers or by moving current register to temporary place
        // (which is more efficient)

        colorizer->exchange_registers(i, colorizer->machine_register_map[old_location]);

        return;
    }

    is_walked[current_location] = true;

    while(true) {
        // What is stored on old place? Can we just move our register to the old place?
        abstract_register bothering_register = colorizer->machine_register_map[old_location];
        if(bothering_register != -1 && bothering_register != i) {
            // No, we can't. We should move bothering register out of our way.
            walk_register_location_graph(is_walked, bothering_register);
            // However, this does not guarantee that this register will be available.
            // We must ensure that it's available, that's why this code is
            // placed inside a cycle.
        } else {
            break;
        }
    }

    is_walked[current_location] = false;

    // Updating descriptor as it may have been changed

    descriptor = get_current_descriptor(i);
    if(descriptor.last_register_location != old_location) {
        // If there is a cycle in the location graph,
        // the above while() cycle will already
        // restore location of current register, so
        // we don't want to waste mov instruction for
        // this
        assert(colorizer->gather_unused_register(old_location));
        colorizer->move_register_to_register(i, old_location);
    }
}

void colorizer_state_pop_context::try_restore_unowned_register_position(abstract_register i) {
    bool is_walked[16] = {};

    walk_register_location_graph(is_walked, i);
}

void colorizer_state_pop_context::restore_register_from_symbol(abstract_register reg, e_machine_register target) {
    abstract_register used_register = colorizer->machine_register_map[target];

    if(used_register != -1) {
        colorizer->release_register_location(used_register, true);
    }

    assert(colorizer->gather_unused_register(target));
    colorizer->restore_register_from_symbol(reg, target);
}

void colorizer_state_pop_context::restore_register_from_memory(abstract_register reg, e_machine_register target) {
    abstract_register used_register = colorizer->machine_register_map[target];

    if(used_register != -1) {
        colorizer->release_register_location(used_register, true);
    }

    assert(colorizer->gather_unused_register(target));
    colorizer->restore_register_from_stack(reg, target);
}

void colorizer_state_pop_context::restore_register_state(abstract_register reg, abstract_register_descriptor old_state, state_register_restore_stage stage) {
    auto current_state = get_current_descriptor(reg);

    if(old_state.located_in_memory) {
        if(current_state.located_in_register) {
            // First step: write all loaded registers back
            if(stage == RESTORE_STAGE_MEM_REG) {
                COLORIZER_DEBUG("Restoring _r%lld (reg -> mem)\n", reg);
                colorizer->move_register_to_stack(reg, current_state.last_memory_position);
            }
        } else if(current_state.located_in_memory) {
            // Do nothing, probably
        } else {
            // Just locate register at its place. We have probably
            // deleted in while popping states somewhere

            colorizer->locate_register_at_stack(reg, old_state.last_memory_position);
        }
    } else if(old_state.located_in_register) {
        if(current_state.located_in_register) {
            // Second step: shuffle all register-stored registers
            if(stage == RESTORE_STAGE_REG_REG) {
                COLORIZER_DEBUG("Restoring _r%lld (reg -> reg)\n", reg);
                try_restore_unowned_register_position(reg);
            }
        } else if(current_state.located_in_memory) {
            if(stage == RESTORE_STAGE_REG_MEM) {
                COLORIZER_DEBUG("Restoring _r%lld (mem -> reg)\n", reg);
                // Third step:
                // We guarantee that old_state.last_register_location is unused for now,
                // since for now each register-stored register is holding its
                // original place on this step
                restore_register_from_memory(reg, old_state.last_register_location);
            }
        } else if(current_state.located_in_symbol) {
            if(stage == RESTORE_STAGE_REG_MEM) {
                restore_register_from_symbol(reg, old_state.last_register_location);
            }
        } else {
            // Just locate register at its place. We have probably
            // deleted in while popping states somewhere

            colorizer->locate_register_at_machine_register(reg, old_state.last_register_location, nullptr);
        }
    } else if(old_state.located_in_symbol) {
        if(current_state.located_in_register) {
            if(stage == RESTORE_STAGE_MEM_REG) {
                colorizer->move_register_to_symbol(reg);
            }
        } else if(!current_state.located_in_symbol) {
            colorizer->locate_register_at_symbol(reg);
        }
    }
}

void colorizer_state_pop_context::restore_registers() {
    for(int stage = 0; stage < 3; stage++) {
        for(int i = 0; i < target_transform.capacity; i++) {
            auto chain = &target_transform.lists[i];
            for(auto j = chain->begin(); j != chain->end(); chain->next_iterator(&j)) {
                hash_table_chain_entry<abstract_register, abstract_register_descriptor>* pair = {};
                chain->get(j, &pair);
                restore_register_state(pair->key, pair->value, state_register_restore_stage(stage));
            }
        }
    }
}

abstract_register_descriptor colorizer_state_pop_context::get_current_descriptor(abstract_register reg) {
    return *colorizer->source->descriptors.get_descriptor(reg);
}

abstract_register_descriptor colorizer_state_pop_context::get_target_descriptor(abstract_register reg) {
    if(target_transform.has(reg)) return target_transform.get(reg);
    return *colorizer->source->descriptors.get_descriptor(reg);
}

bool colorizer_state_pop_context::should_save_register_with_owner(command_list* owner) {
    auto stack = &colorizer->state_stack;
    int pop = 0;
    for(long long i = stack->size - 1; pop < states_to_pop; i--, pop++) {
        if(stack->point(i)->source == owner) return false;
    }
    return true;
}

}
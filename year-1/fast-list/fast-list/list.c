#include "list.h"

const char* MLIST_ERROR_DESCRIPTIONS[] = {
    "MLIST_OK",
    "MLIST_ERROR_LIST_NULL",
    "MLIST_ERROR_INVALID_INDEX",
    "MLIST_ERROR_OUT_OF_MEMORY",
    "MLIST_ERROR_WRONG_SIZE",
    "MLIST_ERROR_CYCLE",
    "MLIST_ERROR_LOST_ELEMENTS",
    "MLIST_ERROR_CORRUPTED_INDICES"
};

s_mlist* mlist_new(mlist_index capacity) {
    s_mlist* list = calloc(1, sizeof(s_mlist));
    
    if(list == NULL) return NULL;
    
    mlist_construct(list, capacity);
    
    return list;
}

e_mlist_error mlist_construct(s_mlist* thou, mlist_index capacity) {
    thou -> tail = 1;
    thou -> head = 1;
    thou -> size = 0;
    thou -> capacity = 0;
    thou -> min_size = capacity;
    thou -> is_mad = true;
    thou -> element_array = NULL;
    thou -> prev_indices = NULL;
    thou -> next_indices = NULL;
    
    if(capacity < 4) {
        thou -> min_size = 4;
    }
    
    e_mlist_error error = mlist_resize(thou, capacity);
    
    if(error) return error;
    
    thou -> next_indices[0] = 1;
    thou -> prev_indices[0] = capacity;
    thou -> prev_indices[1] = 0;
    thou -> next_indices[capacity] = 0;
    
    return MLIST_OK;
}

e_mlist_error mlist_insert(s_mlist* thou, mlist_index index, mlist_element_type element) {
    if(thou == NULL) return MLIST_ERROR_LIST_NULL;
    
    // index = 0 is valid when list is full
    // and user wants to insert element to end
    if(index == 0 && thou -> tail != 0) return MLIST_ERROR_INVALID_INDEX;
    
    // If index = 0 is invalid, it's already has been checked.
    if(index != 0 && index - 1 > thou -> size) return MLIST_ERROR_INVALID_INDEX;
    
    if(thou -> tail == 0) {
        e_mlist_error error = mlist_resize(thou, thou -> capacity * 2);
        if(error) return error;
        
        // If we were adding elements to tail,
        // resetting it to tail as it was zero
        
        if(index == 0) index = thou -> tail;
    }
    
    if(thou -> dumper) mlist_dump_insert_command(thou -> dumper, thou, index, element);
    
    if(index != thou -> tail) thou -> is_mad = false;
    
    mlist_index free_element = thou -> tail;
    
    thou -> element_array[free_element] = element;
    thou -> tail = thou -> next_indices[free_element];
    thou -> size++;
    
    if(index == free_element) {
        if(thou -> dumper) mlist_dumper_add_image(thou -> dumper, thou);
        return MLIST_OK;
    }
    
    thou -> next_indices[thou -> prev_indices[free_element]] = thou -> next_indices[free_element];
    thou -> prev_indices[thou -> next_indices[free_element]] = thou -> prev_indices[free_element];
    
    thou -> prev_indices[free_element] = thou -> prev_indices[index];
    thou -> next_indices[free_element] = index;
    thou -> prev_indices[index] = free_element;
    
    thou -> next_indices[thou -> prev_indices[free_element]] = free_element;
    thou -> prev_indices[thou -> next_indices[index]] = index;
    
    if(thou -> head == index) {
        thou -> head = free_element;
    }
    
    if(thou -> dumper) mlist_dumper_add_image(thou -> dumper, thou);
    
    return MLIST_OK;
}

e_mlist_error mlist_insert_head(s_mlist* thou, mlist_element_type element) {
    return mlist_insert(thou, thou -> head, element);
}

e_mlist_error mlist_insert_tail(s_mlist* thou, mlist_element_type element) {
    return mlist_insert(thou, thou -> tail, element);
}

e_mlist_error mlist_delete(s_mlist* thou, mlist_index index, mlist_element_type* target) {
    if(thou == NULL) return MLIST_ERROR_LIST_NULL;
    if(index == 0) return MLIST_ERROR_INVALID_INDEX;
    if(index > thou -> size) return MLIST_ERROR_INVALID_INDEX;
    
    if(target != NULL) *target = thou -> element_array[index];
    
    if(thou -> dumper) mlist_dump_delete_command(thou -> dumper, thou, index);
    
    if(thou -> head == index && thou -> size != 1) thou -> head = thou -> next_indices[thou -> head];
    
    if(index != thou -> prev_indices[thou -> tail]) thou -> is_mad = false;
    
    mlist_order_swap(thou, index, thou -> size);

    // Deleting item from old position
    
    thou -> next_indices[thou -> prev_indices[thou -> size]] = thou -> next_indices[thou -> size];
    thou -> prev_indices[thou -> next_indices[thou -> size]] = thou -> prev_indices[thou -> size];
    
    // Inserting item to end
    
    thou -> prev_indices[thou -> size] = thou -> prev_indices[thou -> tail];
    thou -> prev_indices[thou -> tail] = thou -> size;
    
    if(thou -> prev_indices[thou -> tail] == thou -> size) {
        thou -> next_indices[thou -> size] = thou -> tail;
        thou -> next_indices[thou -> prev_indices[thou -> size]] = thou -> size;
    } else {
        thou -> next_indices[thou -> size] = thou -> tail;
        thou -> next_indices[thou -> prev_indices[thou -> tail]] = thou -> size;
    }
    
    thou -> tail = thou -> prev_indices[thou -> tail];
    
    thou -> size--;
    
    if(thou -> dumper) mlist_dumper_add_image(thou -> dumper, thou);
    
    if(thou -> size < thou -> capacity / 4 && thou -> capacity > thou -> min_size * 2) {
        mlist_resize(thou, thou -> capacity / 2);
    }
    
    return MLIST_OK;
}

e_mlist_error mlist_delete_head(s_mlist* thou, mlist_element_type* target) {
    return mlist_delete(thou, thou -> head, target);
}

e_mlist_error mlist_delete_tail(s_mlist* thou, mlist_element_type* target) {
    return mlist_delete(thou, thou -> prev_indices[thou -> tail], target);
}

void mlist_order_swap(s_mlist* thou, mlist_index a, mlist_index b) {
    
    if(thou -> head == a) thou -> head = b;
    else if(thou -> head == b) thou -> head = a;
    
    if(thou -> tail == a) thou -> tail = b;
    else if(thou -> tail == b) thou -> tail = a;
    
    mlist_element_type buffer = thou -> element_array[a];
    thou -> element_array[a] = thou -> element_array[b];
    thou -> element_array[b] = buffer;
    
    mlist_index prev_a = thou -> prev_indices[a];
    mlist_index next_a = thou -> next_indices[a];
    
    mlist_index prev_b = thou -> prev_indices[b];
    mlist_index next_b = thou -> next_indices[b];
    
    if(next_a == b) {
        thou -> prev_indices[a] = b;
        thou -> next_indices[b] = a;
    } else {
        thou -> prev_indices[a] = prev_b;
        thou -> next_indices[b] = next_a;
    }
    
    if(prev_a == b) {
        thou -> next_indices[a] = b;
        thou -> prev_indices[b] = a;
    } else {
        thou -> next_indices[a] = next_b;
        thou -> prev_indices[b] = prev_a;
    }
    
    if(next_a != b) {
        thou -> prev_indices[next_a] = b;
        thou -> next_indices[prev_b] = a;
    }
    
    if(prev_a != b) {
        thou -> next_indices[prev_a] = b;
        thou -> prev_indices[next_b] = a;
    }
}

e_mlist_error mlist_engage_madness(s_mlist* thou) {
    if(thou == NULL) return MLIST_ERROR_LIST_NULL;
    
    if(thou -> dumper) mlist_dump_sort_command(thou -> dumper, thou);
    
    mlist_index address = thou -> head, iteration = 1;
    
    for(; address != thou -> tail; address = thou -> next_indices[iteration], iteration++) {
        mlist_index real_address = address;
        mlist_index expected_address = iteration;
        
        mlist_order_swap(thou, real_address, expected_address);
    }
    
    if(thou -> dumper) mlist_dumper_add_image(thou -> dumper, thou);
    
    thou -> is_mad = true;
    
    return MLIST_OK;
}

e_mlist_error mlist_shrink_to_fit(s_mlist* thou) {
    return mlist_resize(thou, thou -> size);
}

e_mlist_error mlist_resize(s_mlist* thou, mlist_index capacity) {
    if(thou == NULL) return MLIST_ERROR_LIST_NULL;
    
    if(thou -> dumper) mlist_dump_resize_command(thou -> dumper, thou, capacity);
    
    capacity++;
    
    mlist_element_type* element_array = realloc(thou -> element_array, capacity * sizeof(mlist_element_type));
    if(!element_array) return MLIST_ERROR_OUT_OF_MEMORY;
    thou -> element_array = element_array;
    
    mlist_index* next_indices = realloc(thou -> next_indices, capacity * sizeof(mlist_index));
    if(!next_indices) return MLIST_ERROR_OUT_OF_MEMORY;
    thou -> next_indices = next_indices;
    
    mlist_index* prev_indices = realloc(thou -> prev_indices, capacity * sizeof(mlist_index));
    if(!prev_indices) return MLIST_ERROR_OUT_OF_MEMORY;
    thou -> prev_indices = prev_indices;
    
    if(capacity > thou -> capacity) {
        
        mlist_index relinker = thou -> capacity;
        
        for(mlist_index index = relinker; index < capacity; index++) {
            thou -> next_indices[index] = index + 1;
            if(index > thou -> capacity) thou -> prev_indices[index] = index - 1;
        }
        
        mlist_index old_last_element = thou -> prev_indices[thou -> tail];
        
        thou -> next_indices[thou -> prev_indices[thou -> tail]] = thou -> capacity;
        thou -> prev_indices[thou -> capacity] = thou -> prev_indices[thou -> tail];
        
        thou -> next_indices[capacity - 1] = 0;
        thou -> prev_indices[0] = capacity - 1;
        thou -> next_indices[0] = thou -> head;
        
        thou -> tail = thou -> next_indices[old_last_element];
    } else {
        if(thou -> tail == capacity) {
            thou -> tail = 0;
        }
        thou -> next_indices[capacity - 1] = 0;
        thou -> prev_indices[0] = capacity - 1;
    }
    
    thou -> capacity = capacity;
    
    if(thou -> dumper) mlist_dumper_add_image(thou -> dumper, thou);
    
    return MLIST_OK;
}

e_mlist_error mlist_set_dumper(s_mlist* thou, s_mlist_dumper* dumper) {
    if(thou == NULL) return MLIST_ERROR_LIST_NULL;
    
    thou -> dumper = dumper;
    mlist_dumper_add_text(dumper, "list initial state\n");
    mlist_dumper_add_image(dumper, thou);
    
    return MLIST_OK;
}

e_mlist_error mlist_linear_address_search(s_mlist* thou, mlist_index index, mlist_index* target) {
    if(thou == NULL) return MLIST_ERROR_LIST_NULL;
    if(index < 0 || index > thou -> size) return MLIST_ERROR_INVALID_INDEX;
    
    if(thou -> is_mad) *target = index + 1;
    
    for(mlist_index address = thou -> head; address != thou -> tail; address = thou -> next_indices[address]) {
        if(index-- == 0) {
            *target = address;
            return MLIST_OK;
        }
    }
    
    if(index == 0) {
        *target = thou -> tail;
        return MLIST_OK;
    }
    
    return MLIST_ERROR_INVALID_INDEX;
}

e_mlist_error mlist_validate(s_mlist* thou) {
    
    mlist_index walked_elements = 0;
    
    for(mlist_index i = thou -> head; i != 0; i = thou -> next_indices[i]) {
        if(i < 0 || i >= thou -> capacity) {
            return MLIST_ERROR_CORRUPTED_INDICES;
        }
        
        if(thou -> tail == i && walked_elements != thou -> size) {
            return MLIST_ERROR_WRONG_SIZE;
        }
        walked_elements++;
        
        if(walked_elements > thou -> capacity) {
            return MLIST_ERROR_CYCLE;
        }
    }
    
    if(walked_elements != thou -> capacity - 1) {
        return MLIST_ERROR_LOST_ELEMENTS;
    }
    
    return MLIST_OK;
}

void mlist_deconstruct(s_mlist* thou) {
    if(thou == NULL) return;
    
    free(thou -> element_array);
    free(thou -> next_indices);
    free(thou -> prev_indices);
}

void mlist_release(s_mlist* thou) {
    mlist_deconstruct(thou);
    free(thou);
}

#include "list.hpp"

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
    auto* list = (s_mlist*)calloc(1, sizeof(s_mlist));
    
    if(list == nullptr) return nullptr;
    
    list->construct(capacity);
    
    return list;
}

e_mlist_error mlist::construct(mlist_index new_capacity) {
    tail = 1;
    head = 1;
    size = 0;
    capacity = 0;
    min_size = new_capacity;
    is_mad = true;
    element_array = nullptr;
    prev_indices = nullptr;
    next_indices = nullptr;
    
    if(capacity < 4) {
        min_size = 4;
    }
    
    e_mlist_error error = resize(new_capacity);
    
    if(error) return error;
    
    next_indices[0] = 1;
    prev_indices[0] = new_capacity;
    prev_indices[1] = 0;
    next_indices[new_capacity] = 0;
    
    return MLIST_OK;
}

e_mlist_error mlist::insert(mlist_index index, mlist_element_type element) {
        
    // index = 0 is valid when list is full
    // and user wants to insert element to end
    if(index == 0 && tail != 0) return MLIST_ERROR_INVALID_INDEX;
    
    // If index = 0 is invalid, it's already has been checked.
    if(index != 0 && index - 1 > size) return MLIST_ERROR_INVALID_INDEX;
    
    if(tail == 0) {
        e_mlist_error error = resize(capacity * 2);
        if(error) return error;
        
        // If we were adding elements to tail,
        // resetting it to tail as it was zero
        
        if(index == 0) index = tail;
    }

    if(index != tail) {
        is_mad = false;
    }

    mlist_index free_element = tail;

    element_array[free_element] = element;
    tail = next_indices[free_element];
    size++;

    if(index == free_element) {
        return MLIST_OK;
    }

    next_indices[prev_indices[free_element]] = next_indices[free_element];
    prev_indices[next_indices[free_element]] = prev_indices[free_element];

    prev_indices[free_element] = prev_indices[index];
    next_indices[free_element] = index;
    prev_indices[index] = free_element;

    next_indices[prev_indices[free_element]] = free_element;
    prev_indices[next_indices[index]] = index;

    if(head == index) {
        head = free_element;
    }


    return MLIST_OK;
}

e_mlist_error mlist::insert_head(mlist_element_type element) {
    return insert(head, element);
}

e_mlist_error mlist::insert_tail(mlist_element_type element) {
    return insert(tail, element);
}

e_mlist_error mlist::delete_index(mlist_index index, mlist_element_type* target) {
    if(index == 0) return MLIST_ERROR_INVALID_INDEX;
    if(index > size) return MLIST_ERROR_INVALID_INDEX;
    
    if(target != nullptr) *target = element_array[index];
    
    if(head == index && size != 1) head = next_indices[head];
    
    if(index != prev_indices[tail]) is_mad = false;
    
    order_swap(index, size);

    // Deleting item from old position
    
    next_indices[prev_indices[size]] = next_indices[size];
    prev_indices[next_indices[size]] = prev_indices[size];
    
    // Inserting item to end
    
    prev_indices[size] = prev_indices[tail];
    prev_indices[tail] = size;
    
    if(prev_indices[tail] == size) {
        next_indices[size] = tail;
        next_indices[prev_indices[size]] = size;
    } else {
        next_indices[size] = tail;
        next_indices[prev_indices[tail]] = size;
    }
    
    tail = prev_indices[tail];
    
    size--;

    
    if(size < capacity / 4 && capacity > min_size * 2) {
        resize(capacity / 2);
    }
    
    return MLIST_OK;
}

e_mlist_error mlist::delete_head(mlist_element_type* target) {
    return delete_index(head, target);
}

e_mlist_error mlist::delete_tail(mlist_element_type* target) {
    return delete_index(prev_indices[tail], target);
}

void mlist::order_swap(mlist_index a, mlist_index b) {
    
    if(head == a) head = b;
    else if(head == b) head = a;
    
    if(tail == a) tail = b;
    else if(tail == b) tail = a;
    
    mlist_element_type buffer = element_array[a];
    element_array[a] = element_array[b];
    element_array[b] = buffer;
    
    mlist_index prev_a = prev_indices[a];
    mlist_index next_a = next_indices[a];
    
    mlist_index prev_b = prev_indices[b];
    mlist_index next_b = next_indices[b];
    
    if(next_a == b) {
        prev_indices[a] = b;
        next_indices[b] = a;
    } else {
        prev_indices[a] = prev_b;
        next_indices[b] = next_a;
    }
    
    if(prev_a == b) {
        next_indices[a] = b;
        prev_indices[b] = a;
    } else {
        next_indices[a] = next_b;
        prev_indices[b] = prev_a;
    }
    
    if(next_a != b) {
        prev_indices[next_a] = b;
        next_indices[prev_b] = a;
    }
    
    if(prev_a != b) {
        next_indices[prev_a] = b;
        prev_indices[next_b] = a;
    }
}

e_mlist_error mlist::engage_madness() {
    
    mlist_index address = head, iteration = 1;
    
    for(; address != tail; address = next_indices[iteration], iteration++) {
        mlist_index real_address = address;
        mlist_index expected_address = iteration;
        
        order_swap(real_address, expected_address);
    }
    
    
    is_mad = true;
    
    return MLIST_OK;
}

e_mlist_error mlist::shrink_to_fit() {
    return resize(size);
}

e_mlist_error mlist::resize(mlist_index new_capacity) {
    new_capacity++;
    
    auto* new_element_array = (mlist_element_type*)realloc(this->element_array, new_capacity * sizeof(mlist_element_type));
    if(!new_element_array) return MLIST_ERROR_OUT_OF_MEMORY;
    element_array = new_element_array;
    
    auto* new_next_indices = (mlist_index*)realloc(this->next_indices, new_capacity * sizeof(mlist_index));
    if(!new_next_indices) return MLIST_ERROR_OUT_OF_MEMORY;
    next_indices = new_next_indices;
    
    auto* new_prev_indices = (mlist_index*)realloc(this->prev_indices, new_capacity * sizeof(mlist_index));
    if(!new_prev_indices) return MLIST_ERROR_OUT_OF_MEMORY;
    prev_indices = new_prev_indices;
    
    if(new_capacity > capacity) {
        
        mlist_index old_capacity = capacity;
        
        for(mlist_index index = old_capacity; index < new_capacity; index++) {
            new_next_indices[index] = index + 1;
            if(index > capacity) prev_indices[index] = index - 1;
        }
        
        mlist_index old_last_element = prev_indices[tail];

        next_indices[prev_indices[tail]] = capacity;
        prev_indices[capacity] = prev_indices[tail];

        next_indices[new_capacity - 1] = 0;
        prev_indices[0] = new_capacity - 1;
        next_indices[0] = head;
        
        tail = next_indices[old_last_element];
    } else {
        if(tail == new_capacity) {
            tail = 0;
        }
        next_indices[capacity - 1] = 0;
        prev_indices[0] = capacity - 1;
    }
    
    capacity = new_capacity;
    
    return MLIST_OK;
}

e_mlist_error mlist::linear_address_search(mlist_index index, mlist_index* target) const {
    if(index < 0 || index > size) return MLIST_ERROR_INVALID_INDEX;
    
    if(is_mad) *target = index + 1;
    
    for(mlist_index address = head; address != tail; address = next_indices[address]) {
        if(index-- == 0) {
            *target = address;
            return MLIST_OK;
        }
    }
    
    if(index == 0) {
        *target = tail;
        return MLIST_OK;
    }
    
    return MLIST_ERROR_INVALID_INDEX;
}

e_mlist_error mlist::validate() const {
    
    mlist_index walked_elements = 0;
    
    for(mlist_index i = head; i != 0; i = next_indices[i]) {
        if(i < 0 || i >= capacity) {
            return MLIST_ERROR_CORRUPTED_INDICES;
        }
        
        if(tail == i && walked_elements != size) {
            return MLIST_ERROR_WRONG_SIZE;
        }
        walked_elements++;
        
        if(walked_elements > capacity) {
            return MLIST_ERROR_CYCLE;
        }
    }
    
    if(walked_elements != capacity - 1) {
        return MLIST_ERROR_LOST_ELEMENTS;
    }
    
    return MLIST_OK;
}

void mlist::deconstruct() {
    free(element_array);
    free(next_indices);
    free(prev_indices);
    element_array = nullptr;
    next_indices = nullptr;
    prev_indices = nullptr;
}

void mlist::release() {
    deconstruct();
    free(this);
}


#include "mlist.hpp"

#ifndef BONK_COMPILER_REINVENTED_MLIST_CPP
#define BONK_COMPILER_REINVENTED_MLIST_CPP

template<typename T>
struct mlist {
    T* element_array;
    auto* prev_indices;
    auto* next_indices;
    auto head;
    auto tail;
    auto capacity;
    auto size;
    auto min_size;

    bool is_mad;

    static mlist* create(auto capacity);

    e_mlist_error construct(auto capacity);
    e_mlist_error insert(auto index, T element);
    e_mlist_error insert_head(T element);
    e_mlist_error insert_tail(T element);
    e_mlist_error delete_index(auto index, T* target);
    e_mlist_error delete_head(T* target);
    e_mlist_error delete_tail(T* target);
    e_mlist_error linear_address_search(auto index, auto* address) const;
    e_mlist_error engage_madness();
    e_mlist_error shrink_to_fit();
    e_mlist_error resize(auto size);

    void order_swap(auto a, auto b);
    void destruct();
    void release();

    void clear();

    e_mlist_error validate();
};

template<typename T>
mlist<T>* mlist<T>::create(auto capacity) {
    DEFAULT_ALLOC_CONSTRUCT(mlist<T>, capacity);
}

template<typename T>
e_mlist_error mlist<T>::construct(auto new_capacity) {
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

template<typename T>
e_mlist_error mlist<T>::insert(auto index, T element) {
        
    // index = 0 is valid when root_list is full
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

    auto free_element = tail;

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

    assert(validate() == MLIST_OK);

    return MLIST_OK;
}

template<typename T>
e_mlist_error mlist<T>::insert_head(T element) {
    return insert(head, element);
}

template<typename T>
e_mlist_error mlist<T>::insert_tail(T element) {
    return insert(tail, element);
}

template<typename T>
e_mlist_error mlist<T>::delete_index(auto index, T* target) {
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

    
//    if(size < capacity / 4 && capacity > min_size * 2) {
//        resize(capacity / 2);
//    }

    assert(validate() == MLIST_OK);
    
    return MLIST_OK;
}

template<typename T>
e_mlist_error mlist<T>::delete_head(T* target) {
    return delete_index(head, target);
}

template<typename T>
e_mlist_error mlist<T>::delete_tail(T* target) {
    return delete_index(prev_indices[tail], target);
}

template<typename T>
void mlist<T>::order_swap(auto a, auto b) {
    
    if(head == a) head = b;
    else if(head == b) head = a;
    
    if(tail == a) tail = b;
    else if(tail == b) tail = a;
    
    T buffer = element_array[a];
    element_array[a] = element_array[b];
    element_array[b] = buffer;
    
    auto prev_a = prev_indices[a];
    auto next_a = next_indices[a];
    
    auto prev_b = prev_indices[b];
    auto next_b = next_indices[b];
    
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

template<typename T>
e_mlist_error mlist<T>::engage_madness() {
    
    auto address = head, iteration = 1;
    
    for(; address != tail; address = next_indices[iteration], iteration++) {
        auto real_address = address;
        auto expected_address = iteration;
        
        order_swap(real_address, expected_address);
    }
    
    
    is_mad = true;
    
    return MLIST_OK;
}

template<typename T>
e_mlist_error mlist<T>::shrink_to_fit() {
    return resize(size);
}

template<typename T>
e_mlist_error mlist<T>::resize(auto new_capacity) {
    new_capacity++;
    
    auto* new_element_array = (T*)realloc(this->element_array, new_capacity * sizeof(T));
    if(!new_element_array) return MLIST_ERROR_OUT_OF_MEMORY;
    element_array = new_element_array;
    
    auto* new_next_indices = (auto*)realloc(this->next_indices, new_capacity * sizeof(auto));
    if(!new_next_indices) return MLIST_ERROR_OUT_OF_MEMORY;
    next_indices = new_next_indices;
    
    auto* new_prev_indices = (auto*)realloc(this->prev_indices, new_capacity * sizeof(auto));
    if(!new_prev_indices) return MLIST_ERROR_OUT_OF_MEMORY;
    prev_indices = new_prev_indices;
    
    if(new_capacity > capacity) {
        
        auto old_capacity = capacity;
        
        for(auto index = old_capacity; index < new_capacity; index++) {
            new_next_indices[index] = index + 1;
            if(index > capacity) prev_indices[index] = index - 1;
        }
        
        auto old_last_element = prev_indices[tail];

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

    assert(validate() == MLIST_OK);
    
    return MLIST_OK;
}

template<typename T>
e_mlist_error mlist<T>::linear_address_search(auto index, auto* target) const {
    if(index < 0 || index > size) return MLIST_ERROR_INVALID_INDEX;
    
    if(is_mad) *target = index + 1;
    
    for(auto address = head; address != tail; address = next_indices[address]) {
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

template<typename T>
void mlist<T>::destruct() {
    free(element_array);
    free(next_indices);
    free(prev_indices);
    element_array = nullptr;
    next_indices = nullptr;
    prev_indices = nullptr;
}

template<typename T>
void mlist<T>::release() {
    destruct();
    free(this);
}

template<typename T>
void mlist<T>::clear() {
    tail = 1;
    head = 1;
    size = 0;
    is_mad = true;
}

template<typename T>
e_mlist_error mlist<T>::validate() {

    auto walked_elements = 0;

    for(auto i = head; i != 0; i = next_indices[i]) {
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

#endif
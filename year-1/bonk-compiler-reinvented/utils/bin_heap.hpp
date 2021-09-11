
#ifndef BONK_COMPILER_REINVENTED_BIN_HEAP_HPP
#define BONK_COMPILER_REINVENTED_BIN_HEAP_HPP

#include "default_alloc_construct.hpp"
#include "dynamic_array.hpp"
#include <cstdlib>

template <typename T>
struct bin_heap {
    dynamic_array<T> buffer;
    long long length;

    static bin_heap* create(long long capacity);
    bool construct(long long capacity);

    void destruct();
    void release();

    T pop();
    void push(T value);
    void sift_down(long long index);
    void sift_up(long long index);

    T get_max();
    T get_min();
};

template <typename T>
bin_heap<T>* bin_heap<T>::create(long long capacity) {
    CHECK_ALLOC_CONSTRUCT(bin_heap<T>, capacity);
}

template <typename T>
bool bin_heap<T>::construct(long long capacity) {
    length = 0;
    if(buffer.construct(capacity)) return false;
    return true;
}

template <typename T>
void bin_heap<T>::destruct() {
    buffer.destruct();
}

template <typename T>
void bin_heap<T>::release() {
    destruct();
    free(this);
}

template <typename T>
T bin_heap<T>::pop() {
    assert(length > 0);
    if(length == 1) {
        length--;
        return buffer.get(0);
    }

    T saved_value = buffer.get(0);
    buffer.set(0, buffer.get(length - 1));
    length--;

    sift_down(0);

    return saved_value;
}

template <typename T>
void bin_heap<T>::push(T value) {
    if(length == 0) {
        if(buffer.size <= length) buffer.push(value);
        else buffer.set(0, value);
        length = 1;
        return;
    }
    long long index = length;
    if(buffer.size <= length) buffer.push(value);
    else buffer.set(index, value);

    length++;

    sift_up(index);
}

template <typename T>
T bin_heap<T>::get_max() {
    return buffer.get(length - 1);
}

template<typename T>
T bin_heap<T>::get_min() {
    return buffer.get(0);
}

template <typename T>
void bin_heap<T>::sift_up(long long index) {
    if(index == 0) {
        return;
    }

    while(true) {
        long long parent_index = (index + 1) / 2 - 1;
        if(parent_index == index) break;
        if(buffer.get(index) < buffer.get(parent_index)) {
            T temp = buffer.get(parent_index);
            buffer.set(parent_index, buffer.get(index));
            buffer.set(index, temp);
        } else {
            break;
        }

        if(parent_index == 0) return;

        index = parent_index;
    }
}

template <typename T>
void bin_heap<T>::sift_down(long long index) {

    while(true) {
        long long child_index = (index + 1) * 2;
        if(child_index > length) {
            break;
        } else if(child_index == length || buffer.get(child_index - 1) < buffer.get(child_index)) {
            child_index--;
        }

        if(child_index == index) {
            return;
        }

        if(buffer.get(child_index) < buffer.get(index)) {
            T temp = buffer.get(index);
            buffer.set(index, buffer.get(child_index));
            buffer.set(child_index, temp);
            index = child_index;
        } else {
            break;
        }
    }
}

#endif //BONK_COMPILER_REINVENTED_BIN_HEAP_HPP

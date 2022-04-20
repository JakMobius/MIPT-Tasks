#pragma once

#include <stdexcept>
#include <new>

template<bool check_range = true>
struct ExceptionStrategy {
    static void on_bad_allocation() {
        throw std::bad_alloc();
    }

    static void verify_index(int index, int size) {
        if(!check_range) return;
        if(index < 0 || index >= size) {
            throw std::range_error("Vector index out of range");
        }
    }
};
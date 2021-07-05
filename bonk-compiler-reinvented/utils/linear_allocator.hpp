//
// Created by Темыч on 25.04.2021.
//

#ifndef BONK_COMPILER_REINVENTED_LINEAR_ALLOCATOR_HPP
#define BONK_COMPILER_REINVENTED_LINEAR_ALLOCATOR_HPP

#include "dynamic_array.hpp"
#include "default_alloc_construct.hpp"

struct linear_allocator_page {
    void* address;
    unsigned long long usage;
    unsigned long long capacity;
};

struct linear_allocator {
    dynamic_array<linear_allocator_page> pages;
    unsigned long long page_capacity;

    static linear_allocator* create(unsigned long long page_capacity);
    bool construct(unsigned long long page_capacity);

    void destruct();
    void release();

    void* allocate(unsigned long long capacity);
    void clear();

    bool create_page(unsigned long long min_capacity);

    char* strdup(const char* str);
};

#endif //BONK_COMPILER_REINVENTED_LINEAR_ALLOCATOR_HPP

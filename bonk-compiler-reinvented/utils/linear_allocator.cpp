//
// Created by Темыч on 25.04.2021.
//

#include <cstring>
#include "linear_allocator.hpp"

linear_allocator* linear_allocator::create(unsigned long long the_page_capacity) {
    CHECK_ALLOC_CONSTRUCT(linear_allocator, the_page_capacity);
}

bool linear_allocator::construct(unsigned long long the_page_capacity) {
    if(pages.construct(4)) return false;
    page_capacity = the_page_capacity;
    return true;
}

void linear_allocator::destruct() {
    pages.destruct();
}

void linear_allocator::release() {
    destruct();
    free(this);
}

void* linear_allocator::allocate(unsigned long long capacity) {
    if(pages.size > 0) {
        linear_allocator_page* page = pages.point(pages.size - 1);
        if(page->capacity - page->usage >= capacity) {
            void* address = (char*) page->address + page->usage;
            page->usage += capacity;
            return address;
        }
    }

    if(!create_page(capacity)) return nullptr;
    linear_allocator_page* page = pages.point(pages.size - 1);
    page->usage += capacity;
    return page->address;
}

void linear_allocator::clear() {
    for(int i = 0; i < pages.size; i++) {
        free(pages.get(i).address);
    }
    pages.clear();
}

bool linear_allocator::create_page(unsigned long long min_capacity) {
    if(min_capacity < page_capacity) min_capacity = page_capacity;

    linear_allocator_page page = {};
    page.usage = 0;
    page.capacity = min_capacity;

    page.address = calloc(1, min_capacity);
    if(!page.address) return false;

    if(pages.push(page)) {
        free(page.address);
        return false;
    }

    return true;
}

char* linear_allocator::strdup(const char* str) {
    char* duplicate = (char*)allocate(strlen(str) + 1);
    strcpy(duplicate, str);
    return duplicate;
}

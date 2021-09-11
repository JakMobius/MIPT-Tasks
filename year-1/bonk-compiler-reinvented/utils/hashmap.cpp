//
// Created by Темыч on 25.04.2021.
//

#include "hashmap.hpp"

template<>
unsigned long long default_hashmap_hash(const char* str) {
    uint64_t hash = 14695981039346656037ull;
    char c = '0';
    int i = 0;

    while((c = str[i++])){
        hash ^= c;
        hash *= 1099511628211ull;
    }

    return hash;
}

template<>
bool default_hashmap_cmp(const char* a, const char* b) {
    return strcmp(a, b) == 0;
}
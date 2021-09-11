
#ifndef hash_table_hpp
#define hash_table_hpp

#include "list.hpp"
#include "cstring"
#include <x86intrin.h>

enum e_hash_table_error {
    HASH_TABLE_OK,
    HASH_TABLE_OUT_OF_MEMORY,
    HASH_TABLE_CHAIN_ERROR,
    HASH_TABLE_FOUND
};

extern "C" unsigned long long string_hash(const char* str);

struct hash_table {
    int capacity;
    int size;
    int max_load_factor;
    mlist* lists;

    e_hash_table_error resize(unsigned long long new_capacity);
    e_hash_table_error insert(const char* key, const char* value, const char** old_value);
    e_hash_table_error erase(const char* key, const char** target);
    e_hash_table_error erase(const char* key, mlist_element_type* target);
    const char* get(const char* key) const;
    mlist_element_type get_pair(const char* key) const;

    e_hash_table_error construct(int new_capacity);
    void destruct();

    static hash_table* create_new();
    double get_collision_factor();
    void release();
};

hash_table* hash_table_new();

#endif /* hash_table_hpp */

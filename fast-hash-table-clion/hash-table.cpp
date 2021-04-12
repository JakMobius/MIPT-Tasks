
#include "hash-table.hpp"

hash_table* hash_table::create_new() {
    return (hash_table*)calloc(1, sizeof(hash_table));
}

e_hash_table_error hash_table::construct(int new_capacity) {
    size = 0;
    capacity = 0;
    lists = nullptr;
    max_load_factor = 250;
    resize(new_capacity);
    return HASH_TABLE_OK;
}

//unsigned long long string_hash(const char* key) {
//    unsigned long long hash = 5381;
//    int c;
//    int l = 64;
//
//    while (l--) {
//        c = (unsigned char)*key++;
//        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */;
//    }
//
//    return hash;
//}

unsigned long long string_hash(const char* key) {
    unsigned long long hash = 0;
    __asm__("xor %0, %0\n"
            "crc32q (%1), %0\n"
            "crc32q 8(%1), %0\n"
            "crc32q 16(%1), %0\n"
            "crc32q 24(%1), %0\n"
            "crc32q 32(%1), %0\n"
            "crc32q 40(%1), %0\n"
            "crc32q 48(%1), %0\n"
            "crc32q 56(%1), %0\n" : "=a" (hash) : "b" (key));
    return hash;
}
//unsigned long long string_hash(const char* str) {
//    uint64_t hash = 14695981039346656037ull;
//
//    for(int i = 0; i < 64; i++) {
//        hash ^= str[i];
//        hash *= 1099511628211ull;
//    }
//
//    return hash;
//}


e_hash_table_error hash_table::resize(unsigned long long new_capacity) {
    auto* new_lists = (mlist*)calloc(new_capacity, sizeof(mlist));
    if(!new_lists) return HASH_TABLE_OUT_OF_MEMORY;

    for(int i = 0; i < new_capacity; i++) {
        new_lists[i].construct(4);
    }

    // Rehash

    for(int i = 0; i < capacity; i++) {
        auto* list = this->lists + i;
        mlist_index head = list->head;
        mlist_index tail = list->tail;
        mlist_index* next = list->next_indices;
        mlist_element_type* storage = list->element_array;

        for(int walker = head; walker != tail; walker = next[walker]) {
            mlist_element_type entry = storage[walker];
            auto error = new_lists[entry.hash % new_capacity].insert_tail(entry);
            if(error) {
                free(new_lists);
                return HASH_TABLE_CHAIN_ERROR;
            }
        }

        list->deconstruct();
    }

    free(lists);
    lists = new_lists;
    capacity = (int)new_capacity;
    return HASH_TABLE_OK;
}

e_hash_table_error hash_table::insert(const char* key, const char* value, const char** old_value) {

    if(value == nullptr) {
        return erase(key, old_value);
    }

    unsigned long long key_hash = string_hash(key);
    unsigned long long chain_index = key_hash % capacity;

    mlist* list = lists + chain_index;

    if(list->size > 0) {
        mlist_index head = list->head;
        mlist_index tail = list->tail;
        mlist_index* next = list->next_indices;
        mlist_element_type* storage = list->element_array;

        for(int walker = head; walker != tail; walker = next[walker]) {
            mlist_element_type* entry = storage + walker;
            if(memcmp(entry->key, key, 64) == 0) {
                if(old_value) *old_value = entry->value;
                entry->value = value;
                return HASH_TABLE_FOUND;
            }
        }
    }

    auto list_error = list->insert_tail({
        .key = key,
        .value = value,
        .hash = key_hash
    });

    if(list_error) return HASH_TABLE_CHAIN_ERROR;

    size++;

    if(capacity * max_load_factor / 100 < size) {
        auto error = resize(capacity * 2);
        if(error) return error;
    }

    if(old_value) *old_value = nullptr;
    return HASH_TABLE_OK;
}

e_hash_table_error hash_table::erase(const char* key, const char** target) {
    mlist_element_type storage = {};

    e_hash_table_error error = erase(key, &storage);
    if(error) return error;
    *target = storage.value;
    return error;
}

e_hash_table_error hash_table::erase(const char* key, mlist_element_type* target = nullptr) {

    unsigned long long key_hash = string_hash(key);
    unsigned long long chain_index = key_hash % capacity;

    mlist* list = this->lists + chain_index;
    mlist_index head = list->head;
    mlist_index tail = list->tail;
    mlist_index* next = list->next_indices;
    mlist_element_type* storage = list->element_array;

    for(int walker = head; walker != tail; walker = next[walker]) {
        mlist_element_type* entry = storage + walker;
        if(memcmp(entry->key, key, 64) == 0) {
            list->delete_index(walker, target);
            size--;
            return HASH_TABLE_FOUND;
        }
    }

    return HASH_TABLE_OK;
}

const char* hash_table::get(const char* key) const {
    mlist_element_type storage = get_pair(key);
    return storage.value;
}

// mlist_element_type hash_table::get_pair(const char* key) const {
//
//     unsigned long long key_hash = string_hash(key);
//     unsigned long long chain_index = key_hash % capacity;
//
//     mlist* list = this->lists + chain_index;
//     mlist_element_type* storage = list->element_array;
//
//     mlist_index head = list->head;
//     mlist_index tail = list->tail;
//     mlist_index* next = list->next_indices;
//
//     for(int walker = head; walker != tail; walker = next[walker]) {
//         mlist_element_type* entry = storage + walker;
//         if(memcmp(entry->key, key, 64) == 0) {
//             return *entry;
//         }
//     }
//
//     return {
//         .key = nullptr,
//         .value = nullptr,
//         .hash = 0
//     };
// }

void hash_table::release() {
    destruct();
    free(this);
}

void hash_table::destruct() {
    for(int i = 0; i < capacity; i++) {
        lists[i].deconstruct();
    }
    free(lists);
}

double hash_table::get_collision_factor() {
    double amount = 0;
    for(int i = 0; i < capacity; i++) {
        int length = lists[i].size;
        if(length > 0) amount++;
    }

    return (double) size / amount;
}



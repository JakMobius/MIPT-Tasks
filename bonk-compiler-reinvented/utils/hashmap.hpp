
#ifndef BONK_COMPILER_REINVENTED_HASHMAP_HPP
#define BONK_COMPILER_REINVENTED_HASHMAP_HPP

#include "./list.hpp"
#include <cstring>

template <typename K, typename T>
struct hash_table_chain_entry {
    K key;
    T value;
    unsigned long long hash;
};

enum e_hash_table_error {
    HASH_TABLE_OK,
    HASH_TABLE_OUT_OF_MEMORY,
    HASH_TABLE_CHAIN_ERROR,
    HASH_TABLE_FOUND
};

template<typename T>
unsigned long long default_hashmap_hash(T value) {
    return (unsigned long long)value;
}

template<typename T>
bool default_hashmap_cmp(T a, T b) {
    return a == b;
}

template<>
unsigned long long default_hashmap_hash(const char* str);

template<>
bool default_hashmap_cmp(const char* a, const char* b);

template <typename K, typename T, unsigned long long (Hash)(K) = default_hashmap_hash<K>, bool (Compare)(K, K) = default_hashmap_cmp<K>>
struct hash_table {
    typedef hash_table_chain_entry<K, T> chain_entry;
    typedef mlist<chain_entry> chain_list;

    int capacity;
    int size;
    mlist<hash_table_chain_entry<K, T>>* lists;

    hash_table<K, T, Hash, Compare>* create(int map_capacity);
    bool construct(int map_capacity);

    void destruct();
    void release();

    e_hash_table_error resize(unsigned long long new_capacity);
    e_hash_table_error insert(K key, T value, T* old_value = nullptr);
    e_hash_table_error erase(K key, T* target);
    e_hash_table_error erase(K key, hash_table_chain_entry<K, T>* target = nullptr);
    T get(K key) const;
    bool has(K key);
    hash_table_chain_entry<K, T>* get_pair(K key) const;

    void clear();
};

template <typename K, typename T, unsigned long long (Hash)(K), bool (Compare)(K, K)>
e_hash_table_error hash_table<K, T, Hash, Compare>::resize(unsigned long long new_capacity) {
    auto* new_lists = (chain_list*)calloc(new_capacity, sizeof(chain_list));
    if(!new_lists) return HASH_TABLE_OUT_OF_MEMORY;

    for(int i = 0; i < new_capacity; i++) {
        new_lists[i].construct(4);
    }

    // Rehash

    for(int i = 0; i < capacity; i++) {
        auto* list = this->lists + i;

        for(auto walker = list->begin(); walker != list->end(); list->next_iterator(&walker)) {
            chain_entry entry = list->get(walker);
            auto error = new_lists[entry.hash % new_capacity].insert_tail(entry);
            if(error) {
                free(new_lists);
                return HASH_TABLE_CHAIN_ERROR;
            }
        }

        list->destruct();
    }

    free(lists);
    lists = new_lists;
    capacity = (int)new_capacity;
    return HASH_TABLE_OK;
}

template <typename K, typename T, unsigned long long (Hash)(K), bool (Compare)(K, K)>
e_hash_table_error hash_table<K, T, Hash, Compare>::insert(K key, T value, T* old_value) {
    unsigned long long key_hash = default_hashmap_hash(key);
    unsigned long long chain_index = key_hash % capacity;

    chain_list* list = lists + chain_index;

    if(list->get_size() > 0) {

        for(auto walker = list->begin(); walker != list->end(); list->next_iterator(&walker)) {
            chain_entry* entry = nullptr;
            list->get(walker, &entry);
            if(Compare(entry->key, key)) {
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

    if(capacity * 100 / 128 < size) {
        auto error = resize(capacity * 2);
        if(error) return error;
    }

    return HASH_TABLE_OK;
}

template <typename K, typename T, unsigned long long (Hash)(K), bool (Compare)(K, K)>
e_hash_table_error hash_table<K, T, Hash, Compare>::erase(K key, T* target) {
    chain_entry storage = {};

    e_hash_table_error error = erase(key, &storage);
    if(error) return error;
    *target = storage.value;
    return error;
}

template <typename K, typename T, unsigned long long (Hash)(K), bool (Compare)(K, K)>
e_hash_table_error hash_table<K, T, Hash, Compare>::erase(K key, chain_entry* target) {

    unsigned long long key_hash = default_hashmap_hash(key);
    unsigned long long chain_index = key_hash % capacity;

    chain_list* list = this->lists + chain_index;

    for(auto walker = list->begin(); walker != list->end(); list->next_iterator(&walker)) {
        chain_entry* entry = nullptr;
        list->get(walker, &entry);
        if(Compare(entry->key, key)) {
            list->get(walker, target);
            list->remove(walker);
            size--;
            return HASH_TABLE_FOUND;
        }
    }

    return HASH_TABLE_OK;
}

template <typename K, typename T, unsigned long long (Hash)(K), bool (Compare)(K, K)>
T hash_table<K, T, Hash, Compare>::get(K key) const {
    chain_entry* storage = get_pair(key);
    return storage->value;
}

template <typename K, typename T, unsigned long long (Hash)(K), bool (Compare)(K, K)>
typename hash_table<K, T, Hash, Compare>::chain_entry* hash_table<K, T, Hash, Compare>::get_pair(K key) const {

    unsigned long long key_hash = default_hashmap_hash(key);
    unsigned long long chain_index = key_hash % capacity;

    chain_list* list = this->lists + chain_index;

    for(auto walker = list->begin(); walker != list->end(); list->next_iterator(&walker)) {
        chain_entry* entry = nullptr;
        list->get(walker, &entry);

        if(Compare(entry->key, key)) {
            return entry;
        }
    }

    return nullptr;
}

template <typename K, typename T, unsigned long long (Hash)(K), bool (Compare)(K, K)>
hash_table<K, T, Hash, Compare>* hash_table<K, T, Hash, Compare>::create(int map_capacity) {
    auto* node = (hash_table<K,T,Hash>*)calloc(1, sizeof(hash_table<K,T,Hash,Compare>));
    if(!node) return nullptr;
    if(!node->construct(map_capacity)) {
        node->destruct();
        return nullptr;
    }
    return node;
}

template <typename K, typename T, unsigned long long (Hash)(K), bool (Compare)(K, K)>
bool hash_table<K, T, Hash, Compare>::construct(int map_capacity) {
    size = 0;
    capacity = 0;
    lists = nullptr;

    if(resize(map_capacity)) return false;
    return true;
}

template <typename K, typename T, unsigned long long (Hash)(K), bool (Compare)(K, K)>
void hash_table<K, T, Hash, Compare>::destruct() {
    for(int i = 0; i < capacity; i++) {
        lists[i].destruct();
    }
    free(lists);
    lists = nullptr;
}

template <typename K, typename T, unsigned long long (Hash)(K), bool (Compare)(K, K)>
void hash_table<K, T, Hash, Compare>::release() {
    destruct();
    free(this);
}

template<typename K, typename T, unsigned long long(Hash)(K), bool(Compare)(K, K)>
void hash_table<K, T, Hash, Compare>::clear() {
    for(int i = 0; i < capacity; i++) {
        lists[i].clear();
    }
    size = 0;
}

template<typename K, typename T, unsigned long long(Hash)(K), bool(Compare)(K, K)>
bool hash_table<K, T, Hash, Compare>::has(K key) {
    return get_pair(key) != nullptr;
}

#endif //BONK_COMPILER_REINVENTED_HASHMAP_HPP

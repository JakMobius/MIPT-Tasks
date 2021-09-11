
#ifndef FAST_HASH_TABLE_CLION_DYNAMIC_ARRAY_HPP
#define FAST_HASH_TABLE_CLION_DYNAMIC_ARRAY_HPP

#include <cstdlib>

enum e_dynamic_array_error {
    DYNAMIC_ARRAY_OK,
    DYNAMIC_ARRAY_OUT_OF_MEMORY
};

template <typename T>
struct s_dynamic_array {
    T* array;
    int capacity;
    int size;

    e_dynamic_array_error construct(int capacity);
    T get(int index);
    e_dynamic_array_error set(int index, T value);
    e_dynamic_array_error push(T value);
    e_dynamic_array_error resize(int capacity);

    void destruct();
    void release();

    static s_dynamic_array<T>* create_new();
};

template<typename T>
T s_dynamic_array<T>::get(int index) {
    return array[index];
}

template<typename T>
e_dynamic_array_error s_dynamic_array<T>::construct(int array_capacity) {
    array = nullptr;
    size = 0;
    capacity = 0;
    return resize(array_capacity);
}

template<typename T>
e_dynamic_array_error s_dynamic_array<T>::set(int index, T value) {
    array[index] = value;
    return DYNAMIC_ARRAY_OK;
}

template<typename T>
e_dynamic_array_error s_dynamic_array<T>::resize(int array_capacity) {
    auto new_array = (T*)realloc(array, array_capacity * sizeof(T));
    if(!new_array) return DYNAMIC_ARRAY_OUT_OF_MEMORY;
    array = new_array;
    capacity = array_capacity;
    return DYNAMIC_ARRAY_OK;
}

template<typename T>
e_dynamic_array_error s_dynamic_array<T>::push(T value) {
    if(size == capacity) {
        auto error = resize(capacity * 2);
        if(error) return error;
    }

    array[size++] = value;
    return DYNAMIC_ARRAY_OK;
}

template<typename T>
s_dynamic_array<T>* s_dynamic_array<T>::create_new() {
    return (s_dynamic_array<T>*)calloc(1, sizeof(s_dynamic_array<T>));
}

template<typename T>
void s_dynamic_array<T>::release() {
    destruct();
    free(this);
}

template<typename T>
void s_dynamic_array<T>::destruct() {
    free(array);
    array = nullptr;
    size = 0;
    capacity = 0;
}

#endif //FAST_HASH_TABLE_CLION_DYNAMIC_ARRAY_HPP

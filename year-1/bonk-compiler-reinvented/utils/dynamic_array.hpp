
#ifndef FAST_HASH_TABLE_CLION_DYNAMIC_ARRAY_HPP
#define FAST_HASH_TABLE_CLION_DYNAMIC_ARRAY_HPP

#include <cstdlib>
#include <cassert>

enum dynamic_array_error {
    DYNAMIC_ARRAY_OK,
    DYNAMIC_ARRAY_OUT_OF_MEMORY
};

template <typename T>
struct dynamic_array {
    T* array;
    int capacity;
    int size;

    static dynamic_array<T>* create();
    dynamic_array_error construct();

    static dynamic_array<T>* create(int capacity);
    dynamic_array_error construct(int capacity);

    static dynamic_array<T>* create(dynamic_array<T>* other);
    dynamic_array_error construct(dynamic_array<T>* other);

    void destruct();
    void release();

    T get(int index);
    T* point(int index);

    T erase(int index);
    dynamic_array_error set(int index, T value);
    dynamic_array_error push(T value);
    dynamic_array_error resize(int capacity);

    void pop();
    void clear();
};

template<typename T>
T dynamic_array<T>::get(int index) {
    assert(index >= 0 && index < capacity);
    return array[index];
}

template<typename T>
T dynamic_array<T>::erase(int index) {
    assert(index >= 0 && index < capacity);
    T value = array[index];

    for(int i = index + 1; i < size; i++) {
        array[i - 1] = array[i];
    }

    size--;

    return value;
}

template<typename T>
T* dynamic_array<T>::point(int index) {
    assert(index >= 0 && index < capacity);
    return array + index;
}

template<typename T>
dynamic_array_error dynamic_array<T>::construct(dynamic_array<T>* other) {
    array = nullptr;
    size = 0;
    capacity = 0;

    int length = other->size;
    if(length < 4) length = 4;

    if(resize(length)) return DYNAMIC_ARRAY_OUT_OF_MEMORY;

    length = other->size;
    for(int i = 0; i < length; i++) {
        push(other->array[i]);
    }

    return DYNAMIC_ARRAY_OK;
}

template<typename T>
dynamic_array_error dynamic_array<T>::construct() {
    return construct(16);
}

template<typename T>
dynamic_array_error dynamic_array<T>::construct(int array_capacity) {
    if(array_capacity < 4) array_capacity = 4;
    array = nullptr;
    size = 0;
    capacity = 0;
    return resize(array_capacity);
}

template<typename T>
dynamic_array_error dynamic_array<T>::set(int index, T value) {
    assert(index >= 0 && index < size);
    array[index] = value;
    return DYNAMIC_ARRAY_OK;
}

template<typename T>
dynamic_array_error dynamic_array<T>::resize(int array_capacity) {
    auto new_array = (T*)realloc(array, array_capacity * sizeof(T));
    if(!new_array) return DYNAMIC_ARRAY_OUT_OF_MEMORY;
    array = new_array;
    capacity = array_capacity;
    return DYNAMIC_ARRAY_OK;
}

template<typename T>
dynamic_array_error dynamic_array<T>::push(T value) {
    if(size == capacity) {
        auto error = resize(capacity * 2);
        if(error) return error;
    }

    array[size++] = value;
    return DYNAMIC_ARRAY_OK;
}

template<typename T>
dynamic_array<T>* dynamic_array<T>::create(int capacity) {
    auto* arr = (dynamic_array<T>*)calloc(1, sizeof(dynamic_array<T>));
    if(!arr) return nullptr;
    if(arr->construct(capacity)) {
        arr->release();
        return nullptr;
    }
    return arr;
}

template<typename T>
dynamic_array<T>* dynamic_array<T>::create() {
    auto* arr = (dynamic_array<T>*)calloc(1, sizeof(dynamic_array<T>));
    if(!arr) return nullptr;
    if(arr->construct()) {
        arr->release();
        return nullptr;
    }
    return arr;
}

template<typename T>
dynamic_array<T>* dynamic_array<T>::create(dynamic_array<T>* other) {
    auto* arr = (dynamic_array<T>*)calloc(1, sizeof(dynamic_array<T>));
    if(!arr) return nullptr;
    if(arr->construct(other)) {
        arr->release();
        return nullptr;
    }
    return arr;
}

template<typename T>
void dynamic_array<T>::release() {
    destruct();
    free(this);
}

template<typename T>
void dynamic_array<T>::destruct() {
    free(array);
    array = nullptr;
    size = 0;
    capacity = 0;
}

template<typename T>
void dynamic_array<T>::pop() {
    size--;
}

template <typename T>
void dynamic_array<T>::clear() {
    size = 0;
}

#endif //FAST_HASH_TABLE_CLION_DYNAMIC_ARRAY_HPP

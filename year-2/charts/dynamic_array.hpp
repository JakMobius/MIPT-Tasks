
#ifndef FAST_HASH_TABLE_CLION_DYNAMIC_ARRAY_HPP
#define FAST_HASH_TABLE_CLION_DYNAMIC_ARRAY_HPP

#include <cstdlib>
#include <cassert>
#include <cstddef>

template <typename T>
struct dynamic_array {
    class out_of_memory {};
    T* array;
    int size;
    int capacity;

    explicit dynamic_array(int capacity = 16);
    dynamic_array(const dynamic_array<T> &other);
    ~dynamic_array();
    dynamic_array<T>& operator=(const dynamic_array<T>& other);

    T get(int index) const;
    T* point(int index) const;

    T erase(int index);
    void set(int index, T value);
    void push(T value);
    void resize(int capacity);

    void pop();
    void clear();
};

template<typename T>
T dynamic_array<T>::get(int index) const {
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
T* dynamic_array<T>::point(int index) const {
    assert(index >= 0 && index < capacity);
    return array + index;
}

template<typename T>
dynamic_array<T>::dynamic_array(const dynamic_array<T> &other) {
    array = nullptr;
    size = 0;
    capacity = 0;

    int length = other.size;
    if(length < 4) length = 4;

    resize(length);

    length = other.size;
    for(int i = 0; i < length; i++) {
        push(other.array[i]);
    }
}

template<typename T>
dynamic_array<T>::dynamic_array(int array_capacity) {
    array = nullptr;
    size = 0;
    capacity = 0;
    if(array_capacity < 4) array_capacity = 4;
    resize(array_capacity);
}

template<typename T>
void dynamic_array<T>::set(int index, T value) {
    assert(index >= 0 && index < size);
    array[index] = value;
}

template<typename T>
void dynamic_array<T>::resize(int array_capacity) {
    T* new_array = (T*)realloc(array, array_capacity * sizeof(T));
    if(!new_array) throw out_of_memory();
    array = new_array;
    capacity = array_capacity;
}

template<typename T>
void dynamic_array<T>::push(T value) {
    if(size == capacity) resize(capacity * 2);

    array[size++] = value;
}

template<typename T>
dynamic_array<T>::~dynamic_array() {
    free(array);
}

template<typename T>
void dynamic_array<T>::pop() {
    size--;
}

template <typename T>
void dynamic_array<T>::clear() {
    size = 0;
}

template<typename T>
dynamic_array<T>& dynamic_array<T>::operator=(const dynamic_array &other) {
    if(&other == this) return *this;
    if(other.capacity > capacity) resize(other.capacity);
    for(int i = 0; i < other.size; i++) array[i] = other.array[i];
    size = other.size;
    return *this;
}

#endif //FAST_HASH_TABLE_CLION_DYNAMIC_ARRAY_HPP

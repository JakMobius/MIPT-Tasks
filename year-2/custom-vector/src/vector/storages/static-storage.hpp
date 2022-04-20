#pragma once

template<typename T, typename ErrorHandlingStrategy, int t_size>
class StaticStorageT {
    T m_storage[t_size];
public:
    typedef T* iterator;
    typedef const T* const_iterator;

    T& operator[](int index) { return m_storage[index]; }
    const T& operator[](int index) const { return m_storage[index]; }
    int capacity() const { return t_size; }
    int size() const { return t_size; }
};

template<int t_size>
struct StaticStorage {
    template<typename T, typename ErrorHandlingStrategy>
    using type = StaticStorageT<T, ErrorHandlingStrategy, t_size>;
};

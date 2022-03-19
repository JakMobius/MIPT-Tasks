#pragma once

#include <algorithm>

template<typename T>
class DynamicStorage {
public:
    typedef T* iterator;
    typedef const T* const_iterator;

    DynamicStorage() = default;

    DynamicStorage(DynamicStorage&& move) noexcept {
        (*this) = std::move(move);
    }

    DynamicStorage& operator=(DynamicStorage&& move_assign)  noexcept {
        m_storage = move_assign.m_storage;
        m_size = move_assign.m_size;
        m_capacity = move_assign.m_capacity;

        move_assign.m_storage = nullptr;
        move_assign.m_size = 0;
        move_assign.m_capacity = 0;
    }

    ~DynamicStorage() {
        free(m_storage);
        m_storage = nullptr;
        m_capacity = 0;
        m_size = 0;
    }

    T& operator[](int index) { return m_storage[index]; }
    const T& operator[](int index) const { return m_storage[index]; }
    [[nodiscard]] int capacity() const { return m_capacity; }
    [[nodiscard]] int size() const { return m_size; }

    void set_capacity(int new_capacity) {
        T* new_storage = nullptr;

        if(new_capacity > 0) new_storage = (T*)calloc(new_capacity, sizeof(T));

        for(int i = new_capacity; i < m_size; i++) {
            (*this)[i].~T();
        }

        if(m_size > new_capacity) {
            m_size = new_capacity;
        }

        for(int i = 0; i < m_size; i++) {
            new(new_storage + i) T(std::move(m_storage[i]));
            m_storage[i].~T();
        }

        m_capacity = new_capacity;
        free(m_storage);
        m_storage = new_storage;
    }

    void set_storage_size(int new_size) {
        m_size = new_size;
    }

protected:
    T* m_storage = nullptr;
    int m_capacity = 0;
    int m_size = 0;
};
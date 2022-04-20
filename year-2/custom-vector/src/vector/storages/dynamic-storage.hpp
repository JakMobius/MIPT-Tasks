#pragma once

#include <algorithm>
#include "../allocators/new-char-allocator.hpp"
#include "../strategies/exception-strategy.hpp"

template<typename T, typename ErrorHandlingStrategy, typename Allocator = NewCharAllocator>
class DynamicStorageT {
public:
    typedef T* iterator;
    typedef const T* const_iterator;

    DynamicStorageT() = default;

    DynamicStorageT(DynamicStorageT&& move) noexcept {
        (*this) = std::move(move);
    }

    DynamicStorageT& operator=(DynamicStorageT&& move_assign)  noexcept {
        if(this == &move_assign) return *this;

        Allocator::free(m_storage);

        m_storage = move_assign.m_storage;
        m_size = move_assign.m_size;
        m_capacity = move_assign.m_capacity;

        move_assign.m_storage = nullptr;
        move_assign.m_size = 0;
        move_assign.m_capacity = 0;

        return *this;
    }

    DynamicStorageT(const DynamicStorageT& copy) noexcept {
        (*this) = copy;
    }

    DynamicStorageT& operator=(const DynamicStorageT& copy_assign)  noexcept {
        if(this == &copy_assign) return *this;

        set_storage_size(0);
        set_capacity(copy_assign.m_capacity);

        for(int i = 0; i < copy_assign.size(); i++) {
            new(m_storage + i) T(copy_assign.m_storage[i]);
        }
        m_size = copy_assign.size();

        return *this;
    }

    ~DynamicStorageT() {
        set_storage_size(0);
        Allocator::free(m_storage);
        m_storage = nullptr;
        m_capacity = 0;
        m_size = 0;
    }

          T& operator[](int index)       { return m_storage[index]; }
    const T& operator[](int index) const { return m_storage[index]; }

    [[nodiscard]] int capacity()   const { return m_capacity; }
    [[nodiscard]] int size()       const { return m_size; }

    void set_capacity(int new_capacity) {
        T* new_storage = create_storage(new_capacity);

        if(m_size > new_capacity) {
            set_storage_size(new_capacity);
        }

        for(int i = 0; i < m_size; i++) {
            new(new_storage + i) T(std::move(m_storage[i]));
            m_storage[i].~T();
        }

        m_capacity = new_capacity;
        Allocator::free(m_storage);
        m_storage = new_storage;
    }

    void set_storage_size(int new_size) {
        for(int i = new_size; i < m_size; i++) (*this)[i].~T();

        m_size = new_size;
    }

protected:
    T* m_storage = nullptr;
    int m_capacity = 0;
    int m_size = 0;

private:
    T* create_storage(int capacity) {
        if(capacity > 0) {
            try {
                return (T*)Allocator::allocate(capacity * sizeof(T));
            } catch(std::bad_alloc& exc) {
                ErrorHandlingStrategy::on_bad_allocation();
            }
        }
        return nullptr;
    }
};

template<typename Allocator = NewCharAllocator>
struct DynamicStorage {
    template<typename T, typename ErrorHandlingStrategy>
    using type = class DynamicStorageT<T, ErrorHandlingStrategy, Allocator>;
};
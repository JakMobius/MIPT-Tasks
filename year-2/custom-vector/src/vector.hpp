#pragma once

#include "dynamic-storage.hpp"

template<typename Vector>
class VectorIterator;

template<typename Vector>
class ConstVectorIterator;

template<typename T, template<typename> typename Storage = DynamicStorage>
class Vector {
    Storage<T> m_storage;
public:
    typedef T& indexed_type;
    typedef const T& const_indexed_type;

    Vector(): m_storage() {}
    Vector(int size): m_storage() {
        resize(size);
    }
    Vector(int size, const T& item): m_storage() {
        resize(size, item);
    }

    Vector(const Vector& copy): m_storage() {
        m_storage.set_capacity(copy.m_size());
        for(auto& element : copy) {
            emplace_back(copy);
        }
    }

    Vector(Vector&& move): m_storage(std::move(move.m_storage)) {}

    void push_back(T&& element) {
        emplace_back(std::move(element));
    }

    void push_back(const T& element) {
        emplace_back(element);
    }

    template<typename ...Args>
    void emplace_back(Args&& ...args) {
        grow_vector(m_storage.size() + 1);
        new (&back()) T { std::forward<Args>(args)... };
    }

    void pop() {
        m_storage.set_size(m_storage.size() - 1);
    }

    T& back() {
        return (*this)[m_storage.size() - 1];
    }

    T& front() {
        return (*this)[0];
    }

    void resize(int new_size) {
        int old_size = m_storage.size();
        resize_vector(new_size);

        for(int i = old_size; i < new_size; i++) new(&(*this)[i]) T();
    }

    void resize(int new_size, const T& item) {
        int old_size = m_storage.size();
        resize_vector(new_size);

        for(int i = old_size; i < new_size; i++) new(&(*this)[i]) T(item);
    }

    T& operator[](int index) { return m_storage[index]; }
    const T& operator[](int index) const { return m_storage[index]; }
    [[nodiscard]] int capacity() const { return m_storage.capacity(); }
    [[nodiscard]] int size() const { return m_storage.size(); }

    void set_capacity(int new_capacity) {}

    VectorIterator<Vector> begin();
    VectorIterator<Vector> end();
    VectorIterator<Vector> rbegin();
    VectorIterator<Vector> rend();

    ConstVectorIterator<Vector> begin() const;
    ConstVectorIterator<Vector> end() const;
    ConstVectorIterator<Vector> rbegin() const;
    ConstVectorIterator<Vector> rend() const;

private:

    void resize_vector(int new_size) {
        if(new_size > m_storage.capacity()) {
            m_storage.set_capacity(new_size);
        }

        for(int i = new_size; i < size(); i++) {
            (*this)[i].~T();
        }

        m_storage.set_storage_size(new_size);
    }

    void grow_vector(int required_size) {
        if(m_storage.capacity() <= required_size) {
            int new_capacity = m_storage.capacity() * 2;
            if(new_capacity < required_size) new_capacity = required_size;
            if(new_capacity < 4) new_capacity = 4;
            m_storage.set_capacity(new_capacity);
        }
        m_storage.set_storage_size(required_size);
    }
};

#include "vector-iterators.hpp"

template<typename T, template<typename> typename Storage>
VectorIterator<Vector<T, Storage>> Vector<T, Storage>::begin() { return { this, 0 }; }

template<typename T, template<typename> typename Storage>
VectorIterator<Vector<T, Storage>> Vector<T, Storage>::end() { return { this, size()}; }

template<typename T, template<typename> typename Storage>
VectorIterator<Vector<T, Storage>> Vector<T, Storage>::rbegin() { return { this, size() - 1 }; }

template<typename T, template<typename> typename Storage>
VectorIterator<Vector<T, Storage>> Vector<T, Storage>::rend() { return { this, -1 }; }

template<typename T, template<typename> typename Storage>
ConstVectorIterator<Vector<T, Storage>> Vector<T, Storage>::begin() const { return { this, 0 }; }

template<typename T, template<typename> typename Storage>
ConstVectorIterator<Vector<T, Storage>> Vector<T, Storage>::end() const { return { this, size() }; }

template<typename T, template<typename> typename Storage>
ConstVectorIterator<Vector<T, Storage>> Vector<T, Storage>::rbegin() const { return { this, size() - 1 }; }

template<typename T, template<typename> typename Storage>
ConstVectorIterator<Vector<T, Storage>> Vector<T, Storage>::rend() const { return { this, -1 }; }

#include "bool/bool-vector.hpp"
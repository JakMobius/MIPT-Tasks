#pragma once

#include "../vector.hpp"

struct ByteProxy;

struct ConstBitProxy {
    uint8_t* m_byte;
    int m_bit;

    operator bool() const {
        return *m_byte & (1 << m_bit);
    }

    bool operator !() const {
        return !((bool)(*this));
    }
};

struct BitProxy : public ConstBitProxy {
    BitProxy& operator=(bool value) {
        uint8_t mask = 1 << m_bit;
        if(value) *m_byte |= mask;
        else *m_byte &= ~mask;
        return *this;
    }
};

struct ByteProxy {
    uint8_t m_byte;

    BitProxy operator[](int index) { return { &m_byte, index }; }
};

template<template<typename> typename Storage>
class Vector<bool, Storage> {
    Storage<ByteProxy> m_storage;
public:
    typedef BitProxy indexed_type;
    typedef ConstBitProxy const_indexed_type;

    Vector(): m_storage() {}
    Vector(int size): m_storage() {
        resize(size);
    }
    Vector(int size, bool item): m_storage() {
        resize(size, item);
    }

    Vector(const Vector& copy): m_storage() {
        m_storage.resize(copy.m_size());

        for(int i = 0; i < m_storage.size(); i++) {
            m_storage[i] = copy.m_storage[i];
        }
        m_real_size = copy.m_size();
    }

    Vector(Vector&& move): m_storage(std::move(move)) {}

    void push_back(bool element) {
        emplace_back(element);
    }

    void emplace_back(bool element) {
        m_real_size++;
        grow_vector(size() + 1);
        back() = element;
    }

    void pop() {
        m_real_size--;
        m_storage.set_size((size() - 1 + 7) / 8);
    }

    BitProxy back() {
        return (*this)[size() - 1];
    }

    ConstBitProxy back() const {
        return (*this)[size() - 1];
    }

    BitProxy front() {
        return (*this)[0];
    }

    ConstBitProxy front() const {
        return (*this)[0];
    }

    void resize(int new_size, bool item = false) {
        int old_size = size();
        m_real_size = new_size;
        resize_vector(new_size);

        for(int i = old_size; i < new_size; i++) (*this)[i] = item;
    }

    BitProxy operator[](int index) { return m_storage[index / 8][index % 8]; }
    ConstBitProxy operator[](int index) const { return m_storage[index / 8][index % 8]; }
    [[nodiscard]] int capacity() const { return m_storage.capacity() * 8; }
    [[nodiscard]] int size() const { return m_real_size; }

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
        new_size = (new_size + 7) / 8;

        if(new_size > m_storage.capacity()) {
            m_storage.set_capacity(new_size);
        }

        m_storage.set_storage_size(new_size);
    }

    void grow_vector(int required_size) {
        required_size = (required_size + 7) / 8;

        if(m_storage.capacity() <= required_size) {
            int new_capacity = m_storage.capacity() * 2;
            if(new_capacity < required_size) new_capacity = required_size;
            if(new_capacity < 4) new_capacity = 4;
            m_storage.set_capacity(new_capacity);
        }
        m_storage.set_storage_size(required_size);
    }

    int m_real_size = 0;
};

template<template<typename> typename Storage>
VectorIterator<Vector<bool, Storage>> Vector<bool, Storage>::begin() { return { this, 0 }; }

template<template<typename> typename Storage>
VectorIterator<Vector<bool, Storage>> Vector<bool, Storage>::end() { return { this, this->size()}; }

template<template<typename> typename Storage>
VectorIterator<Vector<bool, Storage>> Vector<bool, Storage>::rbegin() { return { this, this->size() - 1 }; }

template<template<typename> typename Storage>
VectorIterator<Vector<bool, Storage>> Vector<bool, Storage>::rend() { return { this, -1 }; }

template<template<typename> typename Storage>
ConstVectorIterator<Vector<bool, Storage>> Vector<bool, Storage>::begin() const { return { this, 0 }; }

template<template<typename> typename Storage>
ConstVectorIterator<Vector<bool, Storage>> Vector<bool, Storage>::end() const { return { this, this->size() }; }

template<template<typename> typename Storage>
ConstVectorIterator<Vector<bool, Storage>> Vector<bool, Storage>::rbegin() const { return { this, this->size() - 1 }; }

template<template<typename> typename Storage>
ConstVectorIterator<Vector<bool, Storage>> Vector<bool, Storage>::rend() const { return { this, -1 }; }
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

    BitProxy operator[](int index) { return { &m_byte, index };}
    ConstBitProxy operator[](int index) const { return { const_cast<uint8_t*>(&m_byte), index };}
};

template<
        template<typename ItemT, typename StorageErrorHandlingStrategy> typename Storage,
        typename ErrorHandlingStrategy
        >
class Vector<bool, Storage, ErrorHandlingStrategy> {
    Storage<ByteProxy, ErrorHandlingStrategy> m_storage;
public:
    typedef bool value_type;
    typedef BitProxy indexed_type;
    typedef ConstBitProxy const_indexed_type;

    Vector(): m_storage() {}
    Vector(int size): m_storage() {
        resize(size);
    }
    Vector(int size, bool item): m_storage() {
        resize(size, item);
    }

    Vector(const std::initializer_list<bool>& list): m_storage() {
        m_storage.set_capacity(list.size());
        for(auto& element : list) {
            emplace_back(element);
        }
    }

    Vector(const Vector& copy): m_storage(copy.m_storage) {
        m_real_size = copy.m_real_size;
    }

    Vector(Vector&& move): m_storage(std::move(move.m_storage)), m_real_size(move.m_real_size) {}

    Vector& operator=(const Vector& copy_assign) {
        if(this == &copy_assign) return *this;
        m_storage = copy_assign.m_storage;
        m_real_size = copy_assign.m_real_size;
        return *this;
    }

    Vector& operator=(Vector&& move_assign) {
        if(this == &move_assign) return *this;
        clear();
        m_storage = std::move(move_assign.m_storage);
        m_real_size = move_assign.m_real_size;
        return *this;
    }

    void emplace_back(bool element) {
        m_real_size++;
        grow_vector(size() + 1);
        back() = element;
    }

    void push_back(bool element) { emplace_back(element); }
    void pop_back() { m_real_size--; m_storage.set_size((size() - 1 + 7) / 8); }
    BitProxy back() { return (*this)[size() - 1]; }
    ConstBitProxy back() const { return (*this)[size() - 1]; }
    BitProxy front() { return (*this)[0]; }
    ConstBitProxy front() const { return (*this)[0]; }
    void set_capacity(int new_capacity) { m_storage.set_size((new_capacity - 1 + 7) / 8); }
    void clear() { resize(0); }
    void shrink() { set_capacity(size()); }

    void resize(int new_size, bool item = false) {
        int old_size = size();
        m_real_size = new_size;
        resize_vector(new_size);

        for(int i = old_size; i < new_size; i++) (*this)[i] = item;
    }

    BitProxy operator[](int index) {
        ErrorHandlingStrategy::verify_index(index, size());
        return m_storage[index / 8][index % 8];
    }
    ConstBitProxy operator[](int index) const {
        ErrorHandlingStrategy::verify_index(index, size());
        return m_storage[index / 8][index % 8];
    }
    [[nodiscard]] int capacity() const { return m_storage.capacity() * 8; }
    [[nodiscard]] int size() const { return m_real_size; }

    VectorIterator<Vector>            begin()        { return { this, 0 }; }
    VectorIterator<Vector>            end()          { return { this, this->size()}; }
    VectorIterator<Vector, true>      rbegin()       { return { this, this->size() - 1 }; }
    VectorIterator<Vector, true>      rend()         { return { this, -1 }; }
    ConstVectorIterator<Vector>       begin()  const { return { this, 0 }; }
    ConstVectorIterator<Vector>       end()    const { return { this, this->size() }; }
    ConstVectorIterator<Vector, true> rbegin() const { return { this, this->size() - 1 }; }
    ConstVectorIterator<Vector, true> rend()   const { return { this, -1 }; }

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
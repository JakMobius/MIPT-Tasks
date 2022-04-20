#pragma once

#include "storages/dynamic-storage.hpp"
#include "strategies/exception-strategy.hpp"
#include "iterators/vector-iterators.hpp"

#define VECTOR_TEMPLATE template<  \
    typename T,                    \
    template<                      \
        typename ItemT,            \
        typename StorageErrorHandlingStrategy \
    >                              \
    typename Storage,              \
    typename ErrorHandlingStrategy \
    >

template <
    typename T,
    template<typename StorageT, typename StorageErrorHandlingStrategy> typename Storage = DynamicStorage<>::type,
    typename ErrorHandlingStrategy = ExceptionStrategy<>
    >
class Vector {
    Storage<T, ErrorHandlingStrategy> m_storage;
public:
    typedef T value_type;
    typedef T& indexed_type;
    typedef const T& const_indexed_type;

    Vector(): m_storage() {}
    Vector(int size): m_storage() {
        resize(size);
    }

    Vector(int size, const T& item): m_storage() {
        resize(size, item);
    }

    Vector(const std::initializer_list<T>& list): m_storage() {
        m_storage.set_capacity(list.size());
        for(auto& element : list) {
            emplace_back(element);
        }
    }

    Vector(const Vector& copy): m_storage(copy.m_storage) {}

    Vector(Vector&& move): m_storage(std::move(move.m_storage)) {}

    Vector& operator=(const Vector& copy_assign) {
        if(this == &copy_assign) return *this;
        m_storage = copy_assign.m_storage;
        return *this;
    }

    Vector& operator=(Vector&& move_assign) {
        if(this == &move_assign) return *this;
        clear();
        m_storage = std::move(move_assign.m_storage);
        return *this;
    }

    void push_back(T&& element) { emplace_back(std::move(element)); }
    void push_back(const T& element) { emplace_back(element); }
    void pop_back() { resize(size() - 1); }
    bool empty() { return size() == 0; }
    T& back() { return (*this)[m_storage.size() - 1]; }
    T& front() { return (*this)[0]; }
    void clear() { resize(0); }
    void shrink() { set_capacity(size()); }

    template<typename ...Args>
    void emplace_back(Args&& ...args) {
        grow_vector(m_storage.size() + 1);
        new (&back()) T { std::forward<Args>(args)... };
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

    void set_capacity(int new_capacity) { m_storage.set_capacity(new_capacity); }

    T& operator[](int index) {
        ErrorHandlingStrategy::verify_index(index, size());
        return m_storage[index];
    }
    const T& operator[](int index) const {
        ErrorHandlingStrategy::verify_index(index, size());
        return m_storage[index];
    }
    [[nodiscard]] int capacity() const { return m_storage.capacity(); }
    [[nodiscard]] int size() const { return m_storage.size(); }

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
        if(new_size > m_storage.capacity()) {
            m_storage.set_capacity(new_size);
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

#include "bool/bool-vector.hpp"

template <class Vector>
struct std::iterator_traits<VectorIterator<Vector, false>> {
    typedef typename std::random_access_iterator_tag iterator_category;
    typedef typename Vector::value_type              value_type;
    typedef typename Vector::value_type*             pointer;
    typedef typename Vector::value_type&             reference;
    typedef std::ptrdiff_t                           difference_type;
};

template <class Vector>
struct std::iterator_traits<VectorIterator<Vector, true>> {
    typedef typename std::random_access_iterator_tag iterator_category;
    typedef typename Vector::value_type              value_type;
    typedef typename Vector::value_type*             pointer;
    typedef typename Vector::value_type&             reference;
    typedef std::ptrdiff_t                           difference_type;
};
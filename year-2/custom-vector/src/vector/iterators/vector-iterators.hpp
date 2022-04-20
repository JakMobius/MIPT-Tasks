#pragma once

#include "indexed-iterator.hpp"

template<typename Vector, bool reversed = false>
class ConstVectorIterator : public IndexedIterator<ConstVectorIterator<Vector, reversed>, reversed> {
    const Vector* m_storage;

    friend Vector;
    ConstVectorIterator(const Vector* storage, int index):
        IndexedIterator<ConstVectorIterator<Vector, reversed>>(index),
        m_storage(storage) {}

public:
    typename Vector::const_indexed_type operator*() {
        return (*m_storage)[this->m_index];
    }

    bool operator==(ConstVectorIterator& other) {
        return other.m_storage == m_storage && other.m_index == this->m_index;
    }
};

template<typename Vector, bool reversed = false>
class VectorIterator : public IndexedIterator<VectorIterator<Vector, reversed>, reversed> {
    Vector* m_storage;

    friend Vector;
    VectorIterator(Vector* storage, int index):
        IndexedIterator<VectorIterator<Vector, reversed>,
        reversed>(index),
        m_storage(storage) {}

public:
    typedef typename Vector::indexed_type value_type;

    typename Vector::indexed_type operator*() {
        return (*m_storage)[this->m_index];
    }

    bool operator==(VectorIterator& other) {
        return other.m_storage == m_storage && other.m_index == this->m_index;
    }
};
#pragma once

#include "bool/bool-vector.hpp"
#include "indexed-iterator.hpp"

template<typename Vector>
class ConstVectorIterator : public IndexedIterator<ConstVectorIterator<Vector>> {
    const Vector* m_storage;

    friend Vector;
    ConstVectorIterator(const Vector* storage, int index): IndexedIterator<ConstVectorIterator<Vector>>(index), m_storage(storage) {}

public:
    typename Vector::const_indexed_type operator*() {
        return (*m_storage)[this->m_index];
    }

    bool operator==(ConstVectorIterator& other) {
        return other.m_storage == m_storage && other.m_index == this->m_index;
    }
};

template<typename Vector>
class VectorIterator : public IndexedIterator<VectorIterator<Vector>> {
    Vector* m_storage;

    friend Vector;
    VectorIterator(Vector* storage, int index): IndexedIterator<VectorIterator<Vector>>(index), m_storage(storage) {}

public:
    typename Vector::indexed_type operator*() {
        return (*m_storage)[this->m_index];
    }

    bool operator==(VectorIterator& other) {
        return other.m_storage == m_storage && other.m_index == this->m_index;
    }
};
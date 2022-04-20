#pragma once

#include "../../vector.hpp"
#include <algorithm>

template<typename T, typename ErrorHandlingStrategy, int t_chunk_size, typename Allocator>
class ChunkedStorageT;

template<typename T, int t_chunk_size>
class ChunkedStorageIterator;

template<typename T, int t_chunk_size>
class ConstChunkedStorageIterator;

template<typename T, int t_chunk_size, typename Allocator, typename ErrorHandlingStrategy>
class StorageChunk {
    T* m_storage = nullptr;

public:
    explicit StorageChunk() {
        create_storage();
    }

    StorageChunk(StorageChunk&& move) noexcept {
        *this = std::move(move);
    }

    StorageChunk& operator=(StorageChunk&& move) noexcept {
        if(this == &move) return *this;

        Allocator::free(m_storage);
        m_storage = move.m_storage;
        move.m_storage = nullptr;

        return *this;
    }

    T& operator[](int index) {
        ErrorHandlingStrategy::verify_index(index, t_chunk_size);
        return m_storage[index];
    }

    const T& operator[](int index) const {
        ErrorHandlingStrategy::verify_index(index, t_chunk_size);
        return m_storage[index];
    }

    ~StorageChunk() {
        Allocator::free(m_storage);
        m_storage = nullptr;
    }
private:
    void create_storage() {
        try {
            m_storage = (T*)Allocator::allocate(t_chunk_size * sizeof(T));
        } catch(std::bad_alloc& exc) {
            ErrorHandlingStrategy::on_bad_allocation();
        }
    }
};

template<int t_chunk_size = 512, typename Allocator = NewCharAllocator>
struct ChunkedStorage {
    template<typename T, typename ErrorHandlingStrategy>
    using type = ChunkedStorageT<T, ErrorHandlingStrategy, t_chunk_size, Allocator>;
};

template<typename T, typename ErrorHandlingStrategy, int t_chunk_size, typename Allocator>
class ChunkedStorageT {
public:
    typedef ChunkedStorageIterator<T, t_chunk_size> iterator;
    typedef ChunkedStorageIterator<T, t_chunk_size> const_iterator;

    ChunkedStorageT() {}
    ~ChunkedStorageT() {
        set_storage_size(0);
    }

    ChunkedStorageT(ChunkedStorageT&& move) noexcept {
        (*this) = std::move(move);
    }

    ChunkedStorageT& operator=(ChunkedStorageT&& move_assign)  noexcept {
        if(this == &move_assign) return *this;

        m_chunks = std::move(move_assign.m_chunks);
        m_size = move_assign.m_size;

        move_assign.m_size = 0;
        return *this;
    }

    ChunkedStorageT(const ChunkedStorageT& copy) noexcept {
        (*this) = copy;
    }

    ChunkedStorageT& operator=(const ChunkedStorageT& copy_assign)  noexcept {
        if(this == &copy_assign) return *this;

        set_storage_size(0);
        m_chunks.resize(copy_assign.m_chunks.size());
        m_size = copy_assign.m_size;
        for(int i = 0; i < copy_assign.size(); i++) {
            new(&(*this)[i]) T(copy_assign[i]);
        }

        return *this;
    }

    T& operator[](int index) { return m_chunks[index / t_chunk_size][index % t_chunk_size]; }
    const T& operator[](int index) const { return m_chunks[index / t_chunk_size][index % t_chunk_size]; }
    int capacity() const { return m_chunks.capacity() * t_chunk_size; }
    int size() const { return m_size; }

    void set_capacity(int new_capacity) {
        m_chunks.set_capacity((new_capacity + t_chunk_size - 1) / t_chunk_size);
    }

    void set_storage_size(int new_size) {
        for(int i = new_size; i < m_size; i++) (*this)[i].~T();
        m_chunks.resize((new_size + t_chunk_size - 1) / t_chunk_size);

        m_size = new_size;
    }

protected:
    Vector<
            StorageChunk<T, t_chunk_size, Allocator, ErrorHandlingStrategy>,
            DynamicStorage<Allocator>::template type,
            ErrorHandlingStrategy
        > m_chunks {};
    int m_size = 0;
};

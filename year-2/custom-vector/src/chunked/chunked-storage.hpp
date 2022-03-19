#pragma once

#include "../vector.hpp"
#include <algorithm>

template<typename T, int t_chunk_size>
class ChunkedStorageT;

template<typename T, int t_chunk_size>
class ChunkedStorageIterator;

template<typename T, int t_chunk_size>
class ConstChunkedStorageIterator;

template<typename T, int t_chunk_size>
class StorageChunk {
    T* m_storage = nullptr;
    int m_size = 0;

public:
    explicit StorageChunk() {
        m_size = 0;
        m_storage = (T*)calloc(t_chunk_size, sizeof(T));
    }

    StorageChunk(StorageChunk&& move) noexcept {
        *this = std::move(move);
    }

    StorageChunk& operator=(StorageChunk&& move) noexcept {
        if(this == &move) return *this;
        free(m_storage);
        m_storage = move.m_storage;
        m_size = move.m_size;
        move.m_storage = nullptr;
        move.m_size = 0;
        return *this;
    }

    T& operator[](int index) {
        return m_storage[index];
    }

    const T& operator[](int index) const {
        return m_storage[index];
    }

    ~StorageChunk() {
        free(m_storage);
        m_storage = nullptr;
        m_size = 0;
    }
};

template<int t_chunk_size>
struct ChunkedStorage {
    template<typename T>
    using type = ChunkedStorageT<T, t_chunk_size>;
};

template<typename T, int t_chunk_size = 512>
class ChunkedStorageT {
public:
    typedef ChunkedStorageIterator<T, t_chunk_size> iterator;
    typedef ChunkedStorageIterator<T, t_chunk_size> const_iterator;

    ChunkedStorageT() {}

    ChunkedStorageT(ChunkedStorageT&& move) noexcept {
        (*this) = std::move(move);
    }

    ChunkedStorageT& operator=(ChunkedStorageT&& move_assign)  noexcept {
        m_chunks = std::move(move_assign.m_chunks);
        m_size = move_assign.m_size;

        move_assign.m_size = 0;
    }

    T& operator[](int index) {
        return m_chunks[index / t_chunk_size][index % t_chunk_size];
    }
    const T& operator[](int index) const {
        return m_chunks[index / t_chunk_size][index % t_chunk_size];
    }
    int capacity() const { return m_chunks.capacity() * t_chunk_size; }
    int size() const { return m_size; }

    void set_capacity(int new_capacity) {
        m_chunks.set_capacity((new_capacity + t_chunk_size - 1) / t_chunk_size);
    }

    void set_storage_size(int new_size) {
        m_chunks.resize((new_size + t_chunk_size - 1) / t_chunk_size);
        m_size = new_size;
    }

protected:
    Vector<StorageChunk<T, t_chunk_size>> m_chunks {};
    int m_size = 0;
};

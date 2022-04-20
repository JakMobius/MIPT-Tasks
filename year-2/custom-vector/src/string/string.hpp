#pragma once

#include "../vector/vector.hpp"

struct StringDynamicStorage {
    char* m_storage = nullptr;
    unsigned int m_storage_capacity = 0;
};

#define MAX_SMALL_STRING_SIZE (sizeof(StringDynamicStorage) - 1)
#define INT_HIGH_BIT          (1 << (sizeof(unsigned int) * 8 - 1))

class String {
public:

    String() = default;
    String(const char* literal) {
        resize(strlen(literal));
        strcpy(data(), literal);
    }

    String(const String& copy) {
        (*this) = copy;
    }

    String(String&& move) {
        (*this) = std::move(move);
    }

    String& operator=(String&& move_assign) {
        if(this == &move_assign) return *this;

        clear();

        m_dynamic_storage = move_assign.m_dynamic_storage;
        m_size = move_assign.m_size;

        move_assign.m_dynamic_storage.m_storage = nullptr;
        move_assign.m_dynamic_storage.m_storage_capacity = 0;
        move_assign.m_size = 0;

        return *this;
    }

    String& operator=(const String& copy_assign) {
        if(this == &copy_assign) return *this;

        resize(copy_assign.size());
        strcpy(data(), copy_assign.data());

        return *this;
    }

    void resize(unsigned int size) {
        if(size <= MAX_SMALL_STRING_SIZE) {
            if(storage_is_dynamic()) {
                switch_to_static_storage();
            }
            m_size = size & ~INT_HIGH_BIT;
        } else {
            if(!storage_is_dynamic()) {
                switch_to_dynamic_storage(size);
            } else {
                if(capacity() < size) {
                    set_dynamic_storage_capacity(size);
                }
            }
            m_size = size | INT_HIGH_BIT;
        }
    }

    char* data() {
        if(storage_is_dynamic()) return m_dynamic_storage.m_storage;
        return m_sso_storage;
    }

    const char* data() const {
        return const_cast<String*>(this)->data();
    }

    void clear() {
        if(storage_is_dynamic()) {
            free(m_dynamic_storage.m_storage);
            m_dynamic_storage.m_storage = nullptr;
            m_dynamic_storage.m_storage_capacity = 0;
        }
        m_size = 0;
        mark_storage_static();
    }

    void reserve(unsigned int capacity) {
        if(capacity <= MAX_SMALL_STRING_SIZE) return;
        if(storage_is_dynamic()) {
            set_dynamic_storage_capacity(capacity);
        } else {
            switch_to_dynamic_storage(capacity);
        }
    }

    unsigned int capacity() {
        if(storage_is_dynamic()) return m_dynamic_storage.m_storage_capacity;
        return MAX_SMALL_STRING_SIZE;
    }

    char& operator[](int index) {
        return data()[index];
    }

    inline unsigned int size() const { return m_size & ~INT_HIGH_BIT; }

private:

    inline bool storage_is_dynamic() const { return (m_size & INT_HIGH_BIT) != 0; }
    void mark_storage_dynamic() { m_size |= INT_HIGH_BIT; }
    void mark_storage_static() { m_size &= ~INT_HIGH_BIT; }

    void set_dynamic_storage_capacity(unsigned int capacity) {
        assert(storage_is_dynamic());
        unsigned int old_capacity = m_dynamic_storage.m_storage_capacity;
        m_dynamic_storage.m_storage = (char*) realloc(m_dynamic_storage.m_storage, capacity);
        m_dynamic_storage.m_storage_capacity = capacity;
        for(unsigned int i = old_capacity; i < capacity; i++) {
            m_dynamic_storage.m_storage[i] = '\0';
        }
    }

    void switch_to_dynamic_storage(unsigned int capacity) {
        assert(!storage_is_dynamic());

        char copied_contents[MAX_SMALL_STRING_SIZE + 1];
        strcpy(copied_contents, m_sso_storage);
        m_dynamic_storage.m_storage = nullptr;
        m_dynamic_storage.m_storage_capacity = 0;
        mark_storage_dynamic();
        set_dynamic_storage_capacity(capacity);
        strcpy(m_dynamic_storage.m_storage, copied_contents);
    }

    void switch_to_static_storage() {
        assert(storage_is_dynamic());

        char copied_contents[MAX_SMALL_STRING_SIZE + 1];
        strcpy(copied_contents, m_dynamic_storage.m_storage);
        free(m_dynamic_storage.m_storage);
        mark_storage_static();
    }

    union {
        StringDynamicStorage m_dynamic_storage {};
        char m_sso_storage[0];
    };
    unsigned int m_size = 0;
};

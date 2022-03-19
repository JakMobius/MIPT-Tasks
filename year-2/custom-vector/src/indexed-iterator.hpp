#pragma once

template<typename T>
class IndexedIterator {
protected:
    int m_index = 0;
public:
    explicit IndexedIterator(int index): m_index(index) {};
    IndexedIterator() = default;

    T& operator++() {
        m_index++;
        return *static_cast<T*>(this);
    }

    T operator++(int) {
        T temp = *static_cast<T*>(this);
        ++*this;
        return temp;
    }

    T& operator--() {
        m_index--;
        return *static_cast<T*>(this);
    }

    T operator--(int) {
        T temp = *static_cast<T*>(this);
        --*this;
        return temp;
    }
};
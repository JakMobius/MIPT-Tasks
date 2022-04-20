#pragma once

template<typename T, bool reversed = false>
class IndexedIterator {
protected:
    int m_index = 0;
public:
    explicit IndexedIterator(int index): m_index(index) {};
    IndexedIterator() = default;

    T& operator++() {
        if constexpr(reversed) m_index--;
        else m_index++;

        return *static_cast<T*>(this);
    }

    T operator++(int) {
        T temp = *static_cast<T*>(this);
        ++*this;
        return temp;
    }

    T& operator--() {
        if constexpr(reversed) m_index++;
        else m_index--;

        return *static_cast<T*>(this);
    }

    T operator--(int) {
        T temp = *static_cast<T*>(this);
        --*this;
        return temp;
    }

    T& operator+=(int delta) {
        if constexpr(reversed) this->m_index -= delta;
        else this->m_index += delta;

        return *static_cast<T*>(this);
    }

    T& operator-=(int delta) {
        if constexpr(reversed) this->m_index += delta;
        else this->m_index -= delta;

        return *static_cast<T*>(this);
    }

    T operator+(int delta) {
        T temp = *static_cast<T*>(this);

        temp += delta;

        return temp;
    }

    T operator-(int delta) {
        T temp = *static_cast<T*>(this);

        temp -= delta;

        return temp;
    }

    std::strong_ordering operator<=>(const IndexedIterator& other) const {
        return m_index <=> other.m_index;
    }

    bool operator<(const IndexedIterator& other) const = default;
    bool operator>(const IndexedIterator& other) const = default;
    bool operator==(const IndexedIterator& other) const = default;
    bool operator<=(const IndexedIterator& other) const = default;
    bool operator>=(const IndexedIterator& other) const = default;

    int operator-(const IndexedIterator& other) {
        return m_index - other.m_index;
    }
};
#pragma once

#include <stdexcept>

template<typename T>
using VecValue = T;

template<
    typename T,
    int t_size,
    template<typename ElemT> typename Storage = VecValue
>
struct VecN {
    Storage<T> m_value;
    VecN<T, t_size - 1, Storage> m_rest;

    template<typename ...Rest>
    explicit VecN(const Storage<T>& first, Rest... rest): m_value(first), m_rest(rest...) {}
    VecN(const VecN& copy): m_value(copy.m_value), m_rest(copy.m_rest) {}

    template<int t_index>
    Storage<T>& at() {
        return m_rest.template at<t_index - 1>();
    }

    template<int t_index>
    const Storage<T>& at() const {
        return const_cast<const VecN*>(this)->at<t_index>();
    }

    VecN dot(const VecN& another) const {
        return VecN { at<0>() * another.at<0>(), m_rest.dot(another.m_rest) };
    }

    VecN operator+(const VecN& another) const {
        return VecN { at<0>() + another.at<0>(), m_rest + another.m_rest };
    }

    VecN operator-(const VecN& another) const {
        return VecN { at<0>() - another.at<0>(), m_rest - another.m_rest };
    }

    VecN& operator+=(const VecN& another) {
        at<0>() += another.at<0>();
        m_rest += another.m_rest;
        return *this;
    }

    VecN& operator-=(const VecN& another) {
        at<0>() -= another.at<0>();
        m_rest -= another.m_rest;
        return *this;
    }

    template<>       Storage<T>& at<0>()       { return m_value; }
    template<> const Storage<T>& at<0>() const { return m_value; }

    Storage<T>& operator[](int index) {
        if(index == 0) return m_value;
        return m_rest[index - 1];
    }
    const Storage<T>& operator[](int index) const {
        return (*const_cast<const VecN*>(this))[index];
    }
};

template<
    typename T,
    template<typename ElemT> typename Storage
>
struct VecN<T, 1, Storage> {
    Storage<T> m_value;

    explicit VecN(const Storage<T>& first): m_value(first) {}
    VecN(const VecN& copy): m_value(copy.m_value) {}

    Storage<T>& operator[](int index) {
        if(index == 0) return m_value;
        throw std::out_of_range("VecN index out of range");
    }

    template<int t_index>
    Storage<T>& at() {
        static_assert(t_index == 0, "VecN index out of range");
    }

    template<int t_index>
    const Storage<T>& at() const {
        return const_cast<const VecN*>(this)->at<t_index>();
    }

    VecN dot(const VecN& another)       const { return VecN { at<0>() * another.at<0>() }; }
    VecN operator+(const VecN& another) const { return VecN { at<0>() + another.at<0>() }; }
    VecN operator-(const VecN& another) const { return VecN { at<0>() - another.at<0>() }; }

    VecN& operator+=(const VecN& another) { at<0>() += another.at<0>(); return *this; }
    VecN& operator-=(const VecN& another) { at<0>() -= another.at<0>(); return *this; }

    template<>       Storage<T>& at<0>()       { return m_value; }
    template<> const Storage<T>& at<0>() const { return m_value; }
};
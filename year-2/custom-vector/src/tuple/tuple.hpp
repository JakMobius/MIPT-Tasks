#pragma once

#include <exception>
#include <stdexcept>

template<typename Type1, typename ...RestTypes>
struct Tuple {
    Type1 m_value;
    Tuple<RestTypes...> m_rest;

    explicit Tuple(const Type1& value, const RestTypes&... rest): m_value(value), m_rest(rest...) {

    }

    template<int t_index>
    decltype(auto) at() {
        return m_rest.template at<t_index - 1>();
    }

    template<>
    decltype(auto) at<0>() {
        return m_value;
    }
};

template<typename Type1>
struct Tuple<Type1> {
    Type1 m_value;
    explicit Tuple(const Type1& value): m_value(value) {}

    template<int t_index>
    Type1& at() {
        static_assert(t_index == 0, "Tuple index out of range");
        return m_value;
    }
};
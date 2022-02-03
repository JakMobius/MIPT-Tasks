#pragma once

/**
 * An extremely fast implementation of three-dimensional
 * vector.
 *
 * Authored:                by KingCakeTheFruity
 * Vectorization:           by AlexRoar
 * Debug and inlining help: by JakMobius
 */

#include <cmath>
#include <iostream>
#include "matrix4.hpp"

template<typename T>
struct Vec3 {
    typedef T content3 __attribute__((ext_vector_type(4)));

    union {
        content3 m_content;
        struct {
            T x, y, z;
        };
    };

    Vec3<T>(): m_content {0, 0, 0} {};

    Vec3<T>(T x, T y, T z): m_content {x, y, z} {};

    explicit Vec3<T>(content3 newContent): m_content (newContent) {}

    inline T len() const { return sqrt(len_squared()); }

    inline T len_squared() const {
        const auto squared = m_content * m_content;
        return squared[0] + squared[1] + squared[2];
    }

    inline Vec3<T> normal() const {
        T l = len();
        if(l < 1e-5) return {0, 0, 0};
        else return Vec3<T> {m_content / l};
    }

    inline void normalize() {
        T l = len();
        if(l < 1e-5) m_content = {0, 0, 0};
        else m_content /= l;
    }

    [[nodiscard]] inline bool is_zero() const {
        const auto squared = (m_content * m_content) < 1e-5;
        return squared[0] * squared[1] * squared[2];
    }

    inline T dot(const Vec3<T> &other) const {
        const auto res = (m_content * other.m_content);
        return res[0] + res[1] + res[2];
    }

    inline Vec3<T> cross(const Vec3<T> &other) const {
        const content3 left_first = {m_content[1], m_content[2], m_content[0]};
        const content3 right_first = {other.m_content[2], other.m_content[0], other.m_content[1]};
        const content3 left_second = {m_content[2], m_content[0], m_content[1]};
        const content3 right_second = {other.m_content[1], other.m_content[2], other.m_content[0]};
        const auto res = left_first * right_first - right_second * left_second;
        return Vec3 {res};
    }

    void set_x(T x) { m_content[0] = x; }
    void set_y(T y) { m_content[1] = y; }
    void set_z(T z) { m_content[2] = z; }
    void set(int index, T x) { m_content[index] = x; }

    T operator[](const int i) const { return m_content[i]; }

    inline Vec3<T> operator+() { return *this; }

    inline Vec3<T> operator-() const { return Vec3 {m_content * -1}; }

    inline Vec3<T> operator+(const Vec3<T> &second) const { return Vec3 {m_content + second.m_content}; }

    inline Vec3<T> operator-(const Vec3<T> &second) const { return Vec3 {m_content - second.m_content}; }

    inline Vec3<T> operator*(const Vec3<T> &second) const { return Vec3 {m_content * second.m_content}; }

    inline Vec3<T> operator/(const Vec3<T> &second) const { return Vec3 {m_content / second.m_content}; }

    inline Vec3<T> operator*(const T k) const { return Vec3 {m_content * k}; }

    inline Vec3<T> operator/(T k) const { return Vec3 {m_content / k}; }

    inline Vec3<T> &operator+=(const Vec3<T> &second) { m_content += second.m_content; return *this; }

    inline Vec3<T> &operator-=(const Vec3<T> &second)  { m_content -= second.m_content; return *this; }

    inline Vec3<T> &operator*=(const Vec3<T> &second) { m_content *= second.m_content; return *this; }

    inline Vec3<T> &operator*=(const T k) { m_content *= k; return *this; }

    inline Vec3<T> &operator/=(const Vec3<T> &second) { m_content /= second.m_content; return *this; }

    inline Vec3<T> &operator/=(const T k) { m_content /= k; return *this; }

    void transform_unbound(const Matrix4<T> &matrix) {
        m_content = {
            matrix.transform_x(m_content[0], m_content[1], m_content[2], 0),
            matrix.transform_y(m_content[0], m_content[1], m_content[2], 0),
            matrix.transform_z(m_content[0], m_content[1], m_content[2], 0)
        };
    }

    inline Vec3<T> &operator*=(const Matrix4<T> &other) {

        m_content = {
            other.transform_x(m_content[0], m_content[1], m_content[2]),
            other.transform_y(m_content[0], m_content[1], m_content[2]),
            other.transform_z(m_content[0], m_content[1], m_content[2])
        };

        return *this;
    }

    inline bool operator==(const Vec3<T> &second) const {
        const auto res = m_content - second.m_content;
        return (res[0]) < 1e-5 && (res[1]) < 1e-5 && (res[2]) < 1e-5;
    }

    template<typename V>
    inline explicit operator Vec3<V>() const {
        return Vec3<V> {(V)m_content[0], (V) m_content[1], (V) m_content[2]  };
    }
};

typedef Vec3<double> Vec3d;
typedef Vec3<float> Vec3f;
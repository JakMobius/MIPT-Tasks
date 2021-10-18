#pragma once

/**
 * An extremely fast implementation of two-dimensional
 * vector.
 *
 * Authored:                by KingCakeTheFruity
 * Vectorization:           by AlexRoar
 * Debug and inlining help: by JakMobius
 */

#include <cmath>
#include <iostream>
#include "matrix3.hpp"

extern const float FLOAT_EPS;

template<typename T>
struct Vec2 {
    typedef T content2 __attribute__((ext_vector_type(2)));

    content2 content;

    Vec2<T>(): content {0, 0} {};

    Vec2<T>(T x, T y): content {x, y} {};

    explicit Vec2<T>(content2 newContent): content (newContent) {}

    Vec2<T>(T x_, T y_, T z_): content {x_, y_, z_} {}

    inline T len() const { return sqrt(len_squared()); }

    inline T len_squared() const {
        const auto squared = content * content;
        return squared[0] + squared[1];
    }

    inline Vec2<T> normal() const {
        T l = len();
        if(l < FLOAT_EPS) return {0, 0};
        else return {content / l};
    }

    inline void normalize() {
        T l = len();
        if(l < FLOAT_EPS) content = {0, 0};
        else content /= l;
    }

    [[nodiscard]] inline bool is_zero() const {
        const auto squared = (content * content) < FLOAT_EPS;
        return squared[0] * squared[1];
    }

    inline T dot(const Vec2<T> &other) const {
        const auto res = (content * other.content);
        return res[0] + res[1];
    }

    void set_x(T x) { content[0] = x; }
    void set_y(T y) { content[1] = y; }
    void set(int index, T y) { content[index] = y; }

    T operator[](const int i) const { return content[i]; }

    inline Vec2<T> operator+() { return *this; }

    inline Vec2<T> operator-() const { return {content * -1}; }

    inline Vec2<T> operator+(const Vec2<T> &second) const { return Vec2 {content + second.content}; }

    inline Vec2<T> operator-(const Vec2<T> &second) const { return Vec2 {content - second.content}; }

    inline Vec2<T> operator*(const Vec2<T> &second) const { return Vec2 {content * second.content}; }

    inline Vec2<T> operator/(const Vec2<T> &second) const { return Vec2 {content / second.content}; }

    inline Vec2<T> operator*(const T k) const { return Vec2<T> { content * k }; }

    inline Vec2<T> operator/(T k) const { return Vec2 {content / k}; }

    inline Vec2<T> &operator+=(const Vec2<T> &second) { content += second.content; return *this; }

    inline Vec2<T> &operator-=(const Vec2<T> &second) { content -= second.content; return *this; }

    inline Vec2<T> &operator*=(const Vec2<T> &second) { content *= second.content; return *this; }

    inline Vec2<T> &operator*=(const T k) { content *= k; return *this; }

    inline Vec2<T> &operator/=(const Vec2<T> &second) { content /= second.content; return *this; }

    inline Vec2<T> &operator/=(const T k) { content /= k; return *this; }

    inline Vec2<T> &operator*=(const Matrix3<T> &other) {

        content = {
            other.transform_x(content[0], content[1]),
            other.transform_y(content[0], content[1])
        };

        return *this;
    }

    inline bool operator==(const Vec2<T> &second) const {
        const auto res = content - second.content;
        return (res[0]) < FLOAT_EPS && (res[1]) < FLOAT_EPS;
    }
};

typedef Vec2<double> Vec2d;
typedef Vec2<float> Vec2f;
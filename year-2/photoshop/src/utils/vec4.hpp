#pragma once

#pragma once

/**
 * An extremely fast implementation of four-dimensional
 * vector.
 *
 * Authored:                by KingCakeTheFruity
 * Vectorization:           by AlexRoar
 * Debug and inlining help: by JakMobius
 */

#include <cmath>
#include <iostream>
#include <SFML/Graphics/Color.hpp>
#include "matrix4.hpp"

template<typename T>
struct Vec4 {
    static const T epsilon;
    typedef T content4 __attribute__((ext_vector_type(4)));

    content4 content;

    Vec4<T>(): content {0, 0, 0, 0} {};

    explicit Vec4<T>(content4 newContent): content (newContent) {}

    Vec4<T>(T x, T y, T z, T w): content {x, y, z, w} {}

    inline T len() const { return sqrt(len_squared()); }

    inline T len_squared() const {
        const auto squared = content * content;
        return squared[0] + squared[1] + squared[2] + squared[3];
    }

    inline Vec4<T> normal() const {
        T l = len();
        if(l < epsilon) return {0, 0, 0, 0};
        else return {content / l};
    }

    inline void normalize() {
        T l = len();
        if(l < epsilon) content = {0, 0, 0, 0};
        else content /= l;
    }

    [[nodiscard]] inline bool is_zero() const {
        const auto squared = (content * content) < epsilon;
        return squared[0] * squared[1] * squared[2] * squared[3];
    }

    inline T dot(const Vec4<T> &other) const {
        const auto res = (content * other.content);
        return res[0] + res[1] + res[2] + res[3];
    }

    void set_x(T x) { content[0] = x; }
    void set_y(T x) { content[1] = x; }
    void set_z(T x) { content[2] = x; }
    void set_w(T x) { content[3] = x; }

    T operator[](const int i) const { return content[i]; }

    inline Vec4<T> operator+() { return *this; }

    inline Vec4<T> operator-() const { return {content * -1}; }

    inline Vec4<T> operator+(const Vec4<T> &second) const { return {content + second.content}; }

    inline Vec4<T> operator-(const Vec4<T> &second) const { return {content - second.content}; }

    inline Vec4<T> operator*(const Vec4<T> &second) const { return {content * second.content}; }

    inline Vec4<T> operator/(const Vec4<T> &second) const { return {content / second.content}; }

    inline Vec4<T> operator*(const T k) const { return {content * k}; }

    inline Vec4<T> operator/(T k) const { return {content / k}; }

    inline Vec4<T> &operator+=(const Vec4<T> &second) { content += second.content; return *this; }

    inline Vec4<T> &operator-=(const Vec4<T> &second) { content -= second.content; return *this; }

    inline Vec4<T> &operator*=(const Vec4<T> &second) { content *= second.content; return *this; }

    inline Vec4<T> &operator*=(const T k) { content *= k; return *this; }

    inline Vec4<T> &operator/=(const Vec4<T> &second) { content /= second.content; return *this; }

    inline Vec4<T> &operator/=(const T k) { content /= k; return *this; }

    inline bool operator==(const Vec4<T> &second) const {
        const auto res = content - second.content;
        return (res[0]) < epsilon && (res[1]) < epsilon && (res[2]) < epsilon && res[3] < epsilon;
    }

    sf::Color to_sf_color() { return { (sf::Uint8)(content[0] * 255), (sf::Uint8)(content[1] * 255), (sf::Uint8)(content[2] * 255), (sf::Uint8)(content[3] * 255) }; }
};

typedef Vec4<double> Vec4d;
typedef Vec4<float> Vec4f;
//
// Created by Артем on 01.09.2021.
//

#ifndef GRAPHICS_ZHIRAPHICS_VEC2_HPP
#define GRAPHICS_ZHIRAPHICS_VEC2_HPP

#include <SFML/System/Vector2.hpp>
#include "matrix3.hpp"

class Vec2 {
public:
    float x;
    float y;

    Vec2(float x, float y);
    Vec2(const Vec2& other);
    Vec2();
    ~Vec2();
    Vec2& operator=(const Vec2& other) { x = other.x; y = other.y; return *this; };
    Vec2& operator*=(float other) { multiply(other); return *this; };
    Vec2& operator+=(const Vec2& other) { add(other); return *this; };
    Vec2& operator-=(const Vec2& other) { subtract(other); return *this; };

    Vec2 operator+(const Vec2& other) const { return Vec2(x + other.x, y + other.y); };
    Vec2 operator-(const Vec2& other) const { return Vec2(x - other.x, y - other.y); };
    Vec2 operator*(float coeff) const { return Vec2(x * coeff, y * coeff); }
    Vec2 operator-() const { return Vec2(-x, -y); };

    Vec2& add(const Vec2& other);
    Vec2& subtract(const Vec2& other);
    Vec2& multiply(float scale);

    Vec2& transform(const Matrix3& matrix);
    Vec2& transform(const Matrix3& matrix, float z);

    float length() const;

    sf::Vector2f to_sf_vector() const;

    void set(Vec2 &other);
};

#endif //GRAPHICS_ZHIRAPHICS_VEC2_HPP

#pragma once

#include <SFML/System/Vector2.hpp>
#include <cmath>
#include "matrix3.hpp"

template <typename ValueType>
class Vec2 {
public:
    ValueType x;
    ValueType y;

    Vec2(ValueType x, ValueType y): x(x), y(y) {};
    Vec2(const Vec2 &other) = default;
    Vec2(): Vec2(0, 0) {};
    ~Vec2() = default;

    ValueType dot(const Vec2& other) const { return x * other.x + y * other.y; };

    Vec2& operator=(const Vec2& other) = default;
    Vec2& operator*=(ValueType coefficient) { x *= coefficient; y *= coefficient; return *this; };
    Vec2& operator/=(ValueType coefficient) { x /= coefficient; y /= coefficient; return *this; };
    Vec2& operator+=(const Vec2& other) { x += other.x; y += other.y; return *this; };
    Vec2& operator-=(const Vec2& other) { x -= other.x; y -= other.y; return *this; };

    Vec2 operator+(const Vec2& other) const { return {x + other.x, y + other.y}; };
    Vec2 operator-(const Vec2& other) const { return {x - other.x, y - other.y}; };
    Vec2 operator*(ValueType coefficient) const { return {x * coefficient, y * coefficient}; }
    Vec2 operator/(ValueType coefficient) const { return {x / coefficient, y / coefficient}; }
    Vec2 operator-() const { return {-x, -y}; };

    ValueType length() const { return sqrt(x * x + y * y); };

    sf::Vector2f to_sf_vector() const { return {(float)x, (float)y}; };

    void transform(const Matrix3<ValueType> &matrix);
    void transform(const Matrix3<ValueType> &matrix, ValueType z);
};

template<typename ValueType>
void Vec2<ValueType>::transform(const Matrix3<ValueType> &matrix) {
    float newX = matrix.transform_x(x, y, 1);
    float newY = matrix.transform_y(x, y, 1);

    x = newX;
    y = newY;
}

template<typename ValueType>
void Vec2<ValueType>::transform(const Matrix3<ValueType> &matrix, ValueType z) {
    float newX = matrix.transform_x(x, y, z);
    float newY = matrix.transform_y(x, y, z);

    x = newX;
    y = newY;
}

typedef Vec2<float> Vec2f;
typedef Vec2<double> Vec2d;

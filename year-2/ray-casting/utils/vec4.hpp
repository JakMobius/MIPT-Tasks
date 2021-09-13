#pragma once

#include <SFML/System/Vector2.hpp>
#include <cmath>
#include "matrix4.hpp"

template <typename ValueType>
class Vec4 {
public:
    ValueType x;
    ValueType y;
    ValueType z;
    ValueType w;

    Vec4(ValueType x, ValueType y, ValueType z, ValueType w): x(x), y(y), z(z), w(w) {};
    Vec4(const Vec4 &other) = default;
    Vec4(): Vec4(0, 0, 0, 0) {};
    ~Vec4() = default;

    void multiply_components(const Vec4<ValueType>& other) { x *= other.x; y *= other.y; z *= other.z; w *= other.w; }

    ValueType dot(const Vec4& other) const { return x * other.x + y * other.y + z * other.z + w * other.w; };

    Vec4& operator=(const Vec4& other) = default;
    Vec4& operator*=(ValueType coefficient) { x *= coefficient; y *= coefficient; z *= coefficient; w *= coefficient; return *this; };
    Vec4& operator/=(ValueType coefficient) { x /= coefficient; y /= coefficient; z /= coefficient; w /= coefficient; return *this; };
    Vec4& operator+=(const Vec4& other) { x += other.x; y += other.y; z += other.z; w += other.w; return *this; };
    Vec4& operator-=(const Vec4& other) { x -= other.x; y -= other.y; z -= other.z; w -= other.w; return *this; };

    Vec4 operator+(const Vec4& other) const { return {x + other.x, y + other.y, z + other.z, w + other.w}; };
    Vec4 operator-(const Vec4& other) const { return {x - other.x, y - other.y, z - other.z, w - other.w}; };
    Vec4 operator*(ValueType coefficient) const { return {x * coefficient, y * coefficient, z * coefficient, z * coefficient}; }
    Vec4 operator/(ValueType coefficient) const { return {x / coefficient, y / coefficient, z / coefficient, z / coefficient}; }
    Vec4 operator-() const { return {-x, -y, -z, -w}; };

    ValueType length() const { return sqrt(x * x + y * y + z * z + w * w); };

    sf::Color to_sf_color() { return { (sf::Uint8)(x * 255), (sf::Uint8)(y * 255), (sf::Uint8)(z * 255), (sf::Uint8)(w * 255) }; }
};

typedef Vec4<double> Vec4d;
typedef Vec4<float> Vec4f;
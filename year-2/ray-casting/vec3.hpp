#pragma once

#include <SFML/System/Vector2.hpp>
#include <cmath>
#include "matrix4.hpp"

template <typename ValueType>
class Vec3 {
public:
    ValueType x;
    ValueType y;
    ValueType z;

    Vec3(ValueType x, ValueType y, ValueType z): x(x), y(y), z(z) {};
    Vec3(const Vec3 &other) = default;
    Vec3(): Vec3(0, 0, 0) {};
    ~Vec3() = default;

    ValueType dot(const Vec3& other) const { return x * other.x + y * other.y + z * other.z; };

    Vec3& operator=(const Vec3& other) = default;
    Vec3& operator*=(ValueType coefficient) { x *= coefficient; y *= coefficient; z *= coefficient; return *this; };
    Vec3& operator/=(ValueType coefficient) { x /= coefficient; y /= coefficient; z /= coefficient; return *this; };
    Vec3& operator+=(const Vec3& other) { x += other.x; y += other.y; z += other.z; return *this; };
    Vec3& operator-=(const Vec3& other) { x -= other.x; y -= other.y; z -= other.z; return *this; };

    Vec3 operator+(const Vec3& other) const { return {x + other.x, y + other.y, z + other.z}; };
    Vec3 operator-(const Vec3& other) const { return {x - other.x, y - other.y, z - other.z}; };
    Vec3 operator*(ValueType coefficient) const { return {x * coefficient, y * coefficient, z * coefficient}; }
    Vec3 operator/(ValueType coefficient) const { return {x / coefficient, y / coefficient, z / coefficient}; }
    Vec3 operator-() const { return {-x, -y, -z}; };

    ValueType length() const { return sqrt(x * x + y * y + z * z); };

    Vec3& operator*=(const Matrix4<ValueType>& other);
};

template<typename ValueType>
Vec3<ValueType> &Vec3<ValueType>::operator*=(const Matrix4<ValueType> &other) {
    double new_x = other.transform_x(x, y, z);
    double new_y = other.transform_y(x, y, z);
    double new_z = other.transform_z(x, y, z);

    x = new_x;
    y = new_y;
    z = new_z;

    return *this;
}

typedef Vec3<double> Vec3d;
typedef Vec3<float> Vec3f;

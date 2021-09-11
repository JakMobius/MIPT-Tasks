//
// Created by Артем on 01.09.2021.
//

#include "vec2.hpp"

Vec2::Vec2(): x(0), y(0) {}
Vec2::~Vec2() {}
Vec2::Vec2(const Vec2 &other): x(other.x), y(other.y) {}

Vec2& Vec2::transform(const Matrix3 &matrix) {
    float newX = matrix.transform_x(x, y);
    float newY = matrix.transform_y(x, y);

    x = newX;
    y = newY;

    return *this;
}

Vec2& Vec2::transform(const Matrix3 &matrix, float z) {
    float newX = matrix.transform_x(x, y, z);
    float newY = matrix.transform_y(x, y, z);

    x = newX;
    y = newY;

    return *this;
}

sf::Vector2f Vec2::to_sf_vector() const {
    return sf::Vector2f(x, y);
}

Vec2::Vec2(float x, float y): x(x), y(y) {}

Vec2 &Vec2::add(const Vec2 &other) {
    x += other.x;
    y += other.y;
    return *this;
}

Vec2 &Vec2::subtract(const Vec2 &other) {
    x -= other.x;
    y -= other.y;
    return *this;
}

Vec2 &Vec2::multiply(float scale) {
    x *= scale;
    y *= scale;
    return *this;
}

float Vec2::length() const {
    return sqrt(x * x + y * y);
}

void Vec2::set(Vec2 &other) {
    x = other.x;
    y = other.y;
}


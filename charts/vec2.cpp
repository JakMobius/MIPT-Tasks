//
// Created by Артем on 01.09.2021.
//

#include "vec2.hpp"

Vec2::Vec2(): x(0), y(0) {}
Vec2::~Vec2() {}
Vec2::Vec2(const Vec2 &other): x(other.x), y(other.y) {}
Vec2::Vec2(Vec2 &&other) noexcept: x(other.x), y(other.y) {}

Vec2& Vec2::operator=(Vec2 &&other) noexcept {
    if(&other == this) return *this;
    x = other.x;
    y = other.y;
    return *this;
}

Vec2& Vec2::operator=(const Vec2 &other) {
    if(&other == this) return *this;
    x = other.x;
    y = other.y;
    return *this;
}

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
    return {x, y};
}

Vec2::Vec2(float x, float y): x(x), y(y) {}

Vec2 &Vec2::add(const Vec2 &other) {
    x += other.x;
    y += other.y;
    return *this;
}

Vec2 &Vec2::subtract(const Vec2 &other) {
    x += other.x;
    y += other.y;
    return *this;
}

Vec2 &Vec2::multiply(float scale) {
    x *= scale;
    y *= scale;
    return *this;
}

float Vec2::length() {
    return sqrt(x * x + y * y);
}

Vec2 Vec2::adding(const Vec2& vec2) {
    return Vec2(x + vec2.x, y + vec2.y);
}

Vec2 Vec2::subtracting(const Vec2& vec2) {
    return Vec2(x - vec2.x, y - vec2.y);
}


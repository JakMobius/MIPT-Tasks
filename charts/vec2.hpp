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
    Vec2();
    ~Vec2();
    Vec2(const Vec2& other);
    Vec2(Vec2 &&other) noexcept;
    Vec2& operator=(const Vec2& other);
    Vec2& operator=(Vec2&& other) noexcept;

    Vec2& add(const Vec2& other);
    Vec2& subtract(const Vec2& other);
    Vec2& multiply(float scale);

    Vec2& transform(const Matrix3& matrix);
    Vec2& transform(const Matrix3& matrix, float z);

    float length();

    sf::Vector2f to_sf_vector() const;

    Vec2 adding(const Vec2 &vec2);
    Vec2 subtracting(const Vec2 &vec2);
};

#endif //GRAPHICS_ZHIRAPHICS_VEC2_HPP

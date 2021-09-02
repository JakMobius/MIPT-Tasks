//
// Created by Артем on 01.09.2021.
//

#ifndef GRAPHICS_ZHIRAPHICS_MATRIX3_HPP
#define GRAPHICS_ZHIRAPHICS_MATRIX3_HPP

#include "dynamic_array.hpp"
#include <cmath>

class Matrix3 {
public:
    float data[9];

    Matrix3();
    ~Matrix3();
    Matrix3(const Matrix3& other);
    Matrix3(Matrix3&& other) noexcept;
    Matrix3& operator=(const Matrix3& other);
    Matrix3& operator=(Matrix3&& other) noexcept;

    explicit Matrix3(const float (&data)[9]);

    void multiply(const Matrix3& other);
    Matrix3 multiplied(const Matrix3& other) const;
    Matrix3 inverse() const;

    void set(const float (&data)[9]);

    void scale(float sx, float dy);
    void rotate(float angle);
    void translate(float tx, float ty);

    static Matrix3 scale_matrix(float sx, float sy);
    static Matrix3 rotation_matrix(float angle);
    static Matrix3 translation_matrix(float tx, float ty);

    float transform_x(float x, float y) const;
    float transform_y(float x, float y) const;

    float transform_x(float x, float y, float z) const;
    float transform_y(float x, float y, float z) const;

    void reset();
};

#endif //GRAPHICS_ZHIRAPHICS_MATRIX3_HPP

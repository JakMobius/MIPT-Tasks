//
// Created by Артем on 01.09.2021.
//

#include "matrix3.hpp"

// Constructor
Matrix3::Matrix3() {
    reset();
}

// Destructor
Matrix3::~Matrix3() {}

// Copy constructor
Matrix3::Matrix3(const Matrix3 &other) {
    set(other.data);
}

// Copy assignment constructor
Matrix3& Matrix3::operator=(const Matrix3 &other) {
    if (this == &other) return *this;
    set(other.data);
    return *this;
}

void Matrix3::multiply(const Matrix3 &other) {
    this->set(multiplied(other).data);
}

Matrix3 Matrix3::multiplied(const Matrix3 &other) const {
    Matrix3 result;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++){
            result.data[i * 3 + j] = 0;
            for (int k = 0; k < 3; k++){
                result.data[i * 3 + j] += data[i * 3 + k] * other.data[k * 3 + j];
            }
        }
    }

    return result;
}

Matrix3::Matrix3(const float (&data)[9]): data() {
    set(data);
}

void Matrix3::set(const float (&other)[9]) {
    for(int i = 0; i < 9; i++) data[i] = other[i];
}

Matrix3 Matrix3::inverse() const {
    float t00 = data[4] * data[8] - data[5] * data[7];
    float t10 = data[1] * data[8] - data[2] * data[7];
    float t20 = data[1] * data[5] - data[2] * data[4];
    float d = 1.0f / (data[0] * t00 - data[3] * t10 + data[6] * t20);

    float new_data[9] = {
        d * t00, -d * t10, d * t20,
        -d * (data[3] * data[8] - data[5] * data[6]),
        d *  (data[0] * data[8] - data[2] * data[6]),
        -d * (data[0] * data[5] - data[2] * data[3]),
        d *  (data[3] * data[7] - data[4] * data[6]),
        -d * (data[0] * data[7] - data[1] * data[6]),
        d *  (data[0] * data[4] - data[1] * data[3]),
    };

    return Matrix3(new_data);
}

Matrix3 Matrix3::scale_matrix(float sx, float sy) {
    float scale_matrix_data[9] = {
        sx, 0, 0,
        0, sy, 0,
        0, 0, 1
    };

    return Matrix3(scale_matrix_data);
}

Matrix3 Matrix3::rotation_matrix(float angle) {
    float sine = sin(angle);
    float cosine = cos(angle);

    float rotation_matrix_data[9] = {
        cosine, -sine, 0,
        sine, cosine, 0,
        0, 0, 1
    };

    return Matrix3(rotation_matrix_data);
}

Matrix3 Matrix3::translation_matrix(float tx, float ty) {
    float translation_matrix_data[9] = {
            1, 0, 0,
            0, 1, 0,
            tx, ty, 1
    };

    return Matrix3(translation_matrix_data);
}

void Matrix3::scale(float sx, float sy) {
    multiply(Matrix3::scale_matrix(sx, sy));
}

void Matrix3::rotate(float angle) {
    multiply(Matrix3::rotation_matrix(angle));
}

void Matrix3::translate(float tx, float ty) {
    multiply(Matrix3::translation_matrix(tx, ty));
}

void Matrix3::reset() {
    float default_data[9] = { 1, 0, 0, 0, 1, 0, 0, 0, 1 };
    set(default_data);
}

float Matrix3::transform_x(float x, float y, float z) const {
    return x * data[0] + y * data[3] + z * data[6];
}

float Matrix3::transform_y(float x, float y, float z) const {
    return x * data[1] + y * data[4] + z * data[7];
}

float Matrix3::transform_x(float x, float y) const {
    return x * data[0] + y * data[3] + data[6];
}

float Matrix3::transform_y(float x, float y) const {
    return x * data[1] + y * data[4] + data[7];
}
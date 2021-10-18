#pragma once

#include <cmath>
#include <cstring>

template <typename ValueType>
class Matrix3 {
public:
    ValueType data[9];

    Matrix3();
    ~Matrix3();
    Matrix3(const Matrix3& other);
    Matrix3& operator=(const Matrix3& other);

    explicit Matrix3(const ValueType (&data)[9]);

    void multiply(const Matrix3& other);
    Matrix3 multiplied(const Matrix3& other) const;
    Matrix3 inverse() const;

    void set(const ValueType (&data)[9]);

    void scale(ValueType sx, ValueType dy);
    void rotate(ValueType angle);
    void translate(ValueType tx, ValueType ty);

    static Matrix3 scale_matrix(ValueType sx, ValueType sy);
    static Matrix3 rotation_matrix(ValueType angle);
    static Matrix3 translation_matrix(ValueType tx, ValueType ty);

    ValueType transform_x(ValueType x, ValueType y) const;
    ValueType transform_y(ValueType x, ValueType y) const;

    ValueType transform_x(ValueType x, ValueType y, ValueType z) const;
    ValueType transform_y(ValueType x, ValueType y, ValueType z) const;

    void reset();
};

template <typename ValueType>
Matrix3<ValueType>::Matrix3() {
    reset();
}

template <typename ValueType>
Matrix3<ValueType>::~Matrix3() {}

template <typename ValueType>
Matrix3<ValueType>::Matrix3(const Matrix3<ValueType> &other) {
    set(other.data);
}

template <typename ValueType>
Matrix3<ValueType>& Matrix3<ValueType>::operator=(const Matrix3<ValueType> &other) {
    if (this == &other) return *this;
    set(other.data);
    return *this;
}

template <typename ValueType>
void Matrix3<ValueType>::multiply(const Matrix3<ValueType> &other) {
    this->set(multiplied(other).data);
}

template <typename ValueType>
Matrix3<ValueType> Matrix3<ValueType>::multiplied(const Matrix3<ValueType> &other) const {
    Matrix3<ValueType> result;

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

template <typename ValueType>
Matrix3<ValueType>::Matrix3(const ValueType (&data)[9]): data() {
    set(data);
}

template <typename ValueType>
void Matrix3<ValueType>::set(const ValueType (&other)[9]) {
    for(int i = 0; i < 9; i++) data[i] = other[i];
}

template <typename ValueType>
Matrix3<ValueType> Matrix3<ValueType>::inverse() const {
    ValueType t00 = data[4] * data[8] - data[5] * data[7];
    ValueType t10 = data[1] * data[8] - data[2] * data[7];
    ValueType t20 = data[1] * data[5] - data[2] * data[4];
    ValueType d = 1.0f / (data[0] * t00 - data[3] * t10 + data[6] * t20);

    ValueType new_data[9] = {
            d * t00, -d * t10, d * t20,
            -d * (data[3] * data[8] - data[5] * data[6]),
            d *  (data[0] * data[8] - data[2] * data[6]),
            -d * (data[0] * data[5] - data[2] * data[3]),
            d *  (data[3] * data[7] - data[4] * data[6]),
            -d * (data[0] * data[7] - data[1] * data[6]),
            d *  (data[0] * data[4] - data[1] * data[3]),
    };

    return Matrix3<ValueType>(new_data);
}

template <typename ValueType>
Matrix3<ValueType> Matrix3<ValueType>::scale_matrix(ValueType sx, ValueType sy) {
    ValueType scale_matrix_data[9] = {
            sx, 0, 0,
            0, sy, 0,
            0, 0, 1
    };

    return Matrix3<ValueType>(scale_matrix_data);
}

template <typename ValueType>
Matrix3<ValueType> Matrix3<ValueType>::rotation_matrix(ValueType angle) {
    ValueType sine = sin(angle);
    ValueType cosine = cos(angle);

    ValueType rotation_matrix_data[9] = {
            cosine, -sine, 0,
            sine, cosine, 0,
            0, 0, 1
    };

    return Matrix3<ValueType>(rotation_matrix_data);
}

template <typename ValueType>
Matrix3<ValueType> Matrix3<ValueType>::translation_matrix(ValueType tx, ValueType ty) {
    ValueType translation_matrix_data[9] = {
            1, 0, 0,
            0, 1, 0,
            tx, ty, 1
    };

    return Matrix3<ValueType>(translation_matrix_data);
}

template <typename ValueType>
void Matrix3<ValueType>::scale(ValueType sx, ValueType sy) {
    multiply(Matrix3<ValueType>::scale_matrix(sx, sy));
}

template <typename ValueType>
void Matrix3<ValueType>::rotate(ValueType angle) {
    multiply(Matrix3<ValueType>::rotation_matrix(angle));
}

template <typename ValueType>
void Matrix3<ValueType>::translate(ValueType tx, ValueType ty) {
    multiply(Matrix3<ValueType>::translation_matrix(tx, ty));
}

template <typename ValueType>
void Matrix3<ValueType>::reset() {
    ValueType default_data[9] = { 1, 0, 0, 0, 1, 0, 0, 0, 1 };
    set(default_data);
}

template <typename ValueType>
ValueType Matrix3<ValueType>::transform_x(ValueType x, ValueType y, ValueType z) const {
    return x * data[0] + y * data[3] + z * data[6];
}

template <typename ValueType>
ValueType Matrix3<ValueType>::transform_y(ValueType x, ValueType y, ValueType z) const {
    return x * data[1] + y * data[4] + z * data[7];
}

template <typename ValueType>
ValueType Matrix3<ValueType>::transform_x(ValueType x, ValueType y) const {
    return x * data[0] + y * data[3] + data[6];
}

template <typename ValueType>
ValueType Matrix3<ValueType>::transform_y(ValueType x, ValueType y) const {
    return x * data[1] + y * data[4] + data[7];
}

typedef Matrix3<double> Matrix3d;
typedef Matrix3<float> Matrix3f;

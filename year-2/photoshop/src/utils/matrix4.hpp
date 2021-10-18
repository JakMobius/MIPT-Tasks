#pragma once

#include <cmath>
#include <cstring>

template<typename ValueType>
class Matrix4 {
public:
    ValueType data[16];

    Matrix4() { reset(); };
    ~Matrix4() = default;
    Matrix4(const Matrix4& other) { set(other.data); };
    Matrix4& operator=(const Matrix4& other);

    explicit Matrix4(const ValueType (&data)[16]) { set(data); };

    Matrix4 operator*(const Matrix4& other) const;

    void set(const ValueType (&other)[16]);

    ValueType transform_x(ValueType x, ValueType y, ValueType z) const { return x * data[0] + y * data[4] + z * data[8]; }
    ValueType transform_y(ValueType x, ValueType y, ValueType z) const { return x * data[1] + y * data[5] + z * data[9]; }
    ValueType transform_z(ValueType x, ValueType y, ValueType z) const { return x * data[2] + y * data[6] + z * data[10]; }

    void reset();
};

template<typename ValueType>
Matrix4<ValueType>& Matrix4<ValueType>::operator=(const Matrix4<ValueType> &other) {
    if (this == &other) return *this;
    set(other.data);
    return *this;
}

template<typename ValueType>
Matrix4<ValueType> Matrix4<ValueType>::operator*(const Matrix4<ValueType> &other) const {
    Matrix4<ValueType> result;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++){
            result.data[i * 4 + j] = 0;
            for (int k = 0; k < 4; k++){
                result.data[i * 4 + j] += data[i * 4 + k] * other.data[k * 4 + j];
            }
        }
    }

    return result;
}

template<typename ValueType>
void Matrix4<ValueType>::set(const ValueType (&other)[16]) {
    for(int i = 0; i < 16; i++) data[i] = other[i];
}

template<typename ValueType>
void Matrix4<ValueType>::reset() {
    set({ 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 });
}

typedef Matrix4<double> Matrix4d;
typedef Matrix4<float> Matrix4f;
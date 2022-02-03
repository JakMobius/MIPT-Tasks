#pragma once

#include <cmath>
#include <cstring>

#define SCALE_MATRIX(x, y, z)       { x, 0, 0, 0, 0,  y, 0, 0, 0,  0,  z, 0, 0, 0, 0, 1 }
#define TRANSLATION_MATRIX(x, y, z) { 1, 0, 0, 0, 0,  1, 0, 0, 0,  0,  1, 0, x, y, z, 1 }
#define ROT_X_MATRIX(s, c)      { 1, 0, 0, 0, 0,  c, s, 0, 0,  -s, c, 0, 0, 0, 0, 1 }
#define ROT_Y_MATRIX(s, c)      { c, 0, s, 0, 0,  1, 0, 0, -s, 0,  c, 0, 0, 0, 0, 1 }
#define ROT_Z_MATRIX(s, c)      { c, s, 0, 0, -s, c, 0, 0, 0,  0,  1, 0, 0, 0, 0, 1 }

template<typename ValueType>
class Matrix4 {
public:
    ValueType m_data[16];

    Matrix4() { reset(); };
    ~Matrix4() = default;
    Matrix4(const Matrix4& other) { set(other.m_data); };
    Matrix4& operator=(const Matrix4& other);

    explicit Matrix4(const ValueType (&data)[16]) { set(data); };

    Matrix4 operator*(const Matrix4& other) const;

    void set(const ValueType (&other)[16]);

    ValueType transform_x(ValueType x, ValueType y, ValueType z) const { return x * m_data[0] + y * m_data[4] + z * m_data[8]; }
    ValueType transform_y(ValueType x, ValueType y, ValueType z) const { return x * m_data[1] + y * m_data[5] + z * m_data[9]; }
    ValueType transform_z(ValueType x, ValueType y, ValueType z) const { return x * m_data[2] + y * m_data[6] + z * m_data[10]; }

    Matrix4<ValueType> inverse();

    void set_projection_matrix(ValueType fov, ValueType aspect, ValueType near, ValueType far);
    void set_orthogonal_matrix(ValueType left, ValueType right, ValueType bottom, ValueType top, ValueType near, ValueType far);
    void set_rotation_x_matrix(ValueType angle);
    void set_rotation_y_matrix(ValueType angle);
    void set_rotation_z_matrix(ValueType angle);
    void set_scale_matrix(ValueType x, ValueType y, ValueType z);
    void set_translation_matrix(ValueType x, ValueType y, ValueType z);

    static Matrix4<ValueType> projection_matrix(ValueType fov, ValueType aspect, ValueType near, ValueType far);
    static Matrix4<ValueType> orthogonal_matrix(ValueType left, ValueType right, ValueType bottom, ValueType top, ValueType near, ValueType far);
    static Matrix4<ValueType> rotation_x_matrix(ValueType angle);
    static Matrix4<ValueType> rotation_y_matrix(ValueType angle);
    static Matrix4<ValueType> rotation_z_matrix(ValueType angle);
    static Matrix4<ValueType> scale_matrix(ValueType x, ValueType y, ValueType z);
    static Matrix4<ValueType> translation_matrix(ValueType x, ValueType y, ValueType z);

    void reset();
};

template<typename ValueType>
Matrix4<ValueType>& Matrix4<ValueType>::operator=(const Matrix4<ValueType> &other) {
    if (this == &other) return *this;
    set(other.m_data);
    return *this;
}

template<typename ValueType>
Matrix4<ValueType> Matrix4<ValueType>::operator*(const Matrix4<ValueType> &other) const {
    Matrix4<ValueType> result;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++){
            result.m_data[i * 4 + j] = 0;
            for (int k = 0; k < 4; k++){
                result.m_data[i * 4 + j] += m_data[i * 4 + k] * other.m_data[k * 4 + j];
            }
        }
    }

    return result;
}

template<typename ValueType>
void Matrix4<ValueType>::set_scale_matrix(ValueType x, ValueType y, ValueType z) {
    set(SCALE_MATRIX(x, y, z));
}

template<typename ValueType>
void Matrix4<ValueType>::set_translation_matrix(ValueType x, ValueType y, ValueType z) {
    set(TRANSLATION_MATRIX(x, y, z));
}

template<typename ValueType>
void Matrix4<ValueType>::set_rotation_x_matrix(ValueType angle) {
    ValueType s = sin(angle);
    ValueType c = cos(angle);
    set(ROT_X_MATRIX(s, c));
}

template<typename ValueType>
void Matrix4<ValueType>::set_rotation_y_matrix(ValueType angle) {
    ValueType s = sin(angle);
    ValueType c = cos(angle);
    set(ROT_Y_MATRIX(s, c));
}

template<typename ValueType>
void Matrix4<ValueType>::set_rotation_z_matrix(ValueType angle) {
    ValueType s = sin(angle);
    ValueType c = cos(angle);
    set(ROT_Z_MATRIX(s, c));
}

template<typename ValueType>
Matrix4<ValueType> Matrix4<ValueType>::scale_matrix(ValueType x, ValueType y, ValueType z) {
    return Matrix4(SCALE_MATRIX(x, y, z));
}

template<typename ValueType>
Matrix4<ValueType> Matrix4<ValueType>::translation_matrix(ValueType x, ValueType y, ValueType z) {
    return Matrix4(TRANSLATION_MATRIX(x, y, z));
}

template<typename ValueType>
Matrix4<ValueType> Matrix4<ValueType>::rotation_x_matrix(ValueType angle) {
    ValueType s = sin(angle);
    ValueType c = cos(angle);
    return Matrix4(ROT_X_MATRIX(s, c));
}

template<typename ValueType>
Matrix4<ValueType> Matrix4<ValueType>::rotation_y_matrix(ValueType angle) {
    ValueType s = sin(angle);
    ValueType c = cos(angle);
    return Matrix4(ROT_Y_MATRIX(s, c));
}

template<typename ValueType>
Matrix4<ValueType> Matrix4<ValueType>::rotation_z_matrix(ValueType angle) {
    ValueType s = sin(angle);
    ValueType c = cos(angle);
    return Matrix4(ROT_Z_MATRIX(s, c));
}

template<typename ValueType>
void Matrix4<ValueType>::set_projection_matrix(ValueType fov, ValueType aspect, ValueType near, ValueType far) {

    ValueType y_scale = 1.0 / tan(fov / 2);
    ValueType x_scale = y_scale / aspect;
    ValueType dist = near - far;

    set({
          x_scale, 0,       0,                    0,
          0,       y_scale, 0,                    0,
          0,       0,       (far + near) / dist,  -1,
          0,       0,       2 * far * near / dist, 0
    });
}

template<typename ValueType>
void Matrix4<ValueType>::set_orthogonal_matrix(ValueType left, ValueType right, ValueType bottom, ValueType top, ValueType near, ValueType far) {

    set({
        2.0f / (right - left), 0,                     0,                   0,
        0,                     2.0f / (top - bottom), 0,                   0,
        0,                     0,                     -2.0f / (far - near),0,
        0,                     0,                     0,                   1
    });
}

template<typename ValueType>
Matrix4<ValueType> Matrix4<ValueType>::projection_matrix(ValueType fov, ValueType aspect, ValueType near, ValueType far) {
    Matrix4<ValueType> matrix {};
    matrix.set_projection_matrix(fov, aspect, near, far);
    return matrix;
}

template<typename ValueType>
Matrix4<ValueType> Matrix4<ValueType>::orthogonal_matrix(ValueType left, ValueType right, ValueType bottom, ValueType top, ValueType near, ValueType far) {
    Matrix4<ValueType> matrix {};
    matrix.set_orthogonal_matrix(left, right, bottom, top, near, far);
    return matrix;
}


template<typename ValueType>
void Matrix4<ValueType>::set(const ValueType (&other)[16]) {
    for(int i = 0; i < 16; i++) m_data[i] = other[i];
}

template<typename ValueType>
void Matrix4<ValueType>::reset() {
    set({ 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 });
}

template<typename ValueType>
Matrix4<ValueType> Matrix4<ValueType>::inverse() {
    ValueType a_2323 = m_data[2 * 4 + 2] * m_data[3 * 4 + 3] - m_data[2 * 4 + 3] * m_data[3 * 4 + 2];
    ValueType a_1323 = m_data[2 * 4 + 1] * m_data[3 * 4 + 3] - m_data[2 * 4 + 3] * m_data[3 * 4 + 1];
    ValueType a_1223 = m_data[2 * 4 + 1] * m_data[3 * 4 + 2] - m_data[2 * 4 + 2] * m_data[3 * 4 + 1];
    ValueType a_0323 = m_data[2 * 4 + 0] * m_data[3 * 4 + 3] - m_data[2 * 4 + 3] * m_data[3 * 4 + 0];
    ValueType a_0223 = m_data[2 * 4 + 0] * m_data[3 * 4 + 2] - m_data[2 * 4 + 2] * m_data[3 * 4 + 0];
    ValueType a_0123 = m_data[2 * 4 + 0] * m_data[3 * 4 + 1] - m_data[2 * 4 + 1] * m_data[3 * 4 + 0];
    ValueType a_2313 = m_data[1 * 4 + 2] * m_data[3 * 4 + 3] - m_data[1 * 4 + 3] * m_data[3 * 4 + 2];
    ValueType a_1313 = m_data[1 * 4 + 1] * m_data[3 * 4 + 3] - m_data[1 * 4 + 3] * m_data[3 * 4 + 1];
    ValueType a_1213 = m_data[1 * 4 + 1] * m_data[3 * 4 + 2] - m_data[1 * 4 + 2] * m_data[3 * 4 + 1];
    ValueType a_2312 = m_data[1 * 4 + 2] * m_data[2 * 4 + 3] - m_data[1 * 4 + 3] * m_data[2 * 4 + 2];
    ValueType a_1312 = m_data[1 * 4 + 1] * m_data[2 * 4 + 3] - m_data[1 * 4 + 3] * m_data[2 * 4 + 1];
    ValueType a_1212 = m_data[1 * 4 + 1] * m_data[2 * 4 + 2] - m_data[1 * 4 + 2] * m_data[2 * 4 + 1];
    ValueType a_0313 = m_data[1 * 4 + 0] * m_data[3 * 4 + 3] - m_data[1 * 4 + 3] * m_data[3 * 4 + 0];
    ValueType a_0213 = m_data[1 * 4 + 0] * m_data[3 * 4 + 2] - m_data[1 * 4 + 2] * m_data[3 * 4 + 0];
    ValueType a_0312 = m_data[1 * 4 + 0] * m_data[2 * 4 + 3] - m_data[1 * 4 + 3] * m_data[2 * 4 + 0];
    ValueType a_0212 = m_data[1 * 4 + 0] * m_data[2 * 4 + 2] - m_data[1 * 4 + 2] * m_data[2 * 4 + 0];
    ValueType a_0113 = m_data[1 * 4 + 0] * m_data[3 * 4 + 1] - m_data[1 * 4 + 1] * m_data[3 * 4 + 0];
    ValueType a_0112 = m_data[1 * 4 + 0] * m_data[2 * 4 + 1] - m_data[1 * 4 + 1] * m_data[2 * 4 + 0];

    ValueType det = + m_data[0 * 4 + 0] * ( m_data[1 * 4 + 1] * a_2323 - m_data[1 * 4 + 2] * a_1323 + m_data[1 * 4 + 3] * a_1223)
                    - m_data[0 * 4 + 1] * ( m_data[1 * 4 + 0] * a_2323 - m_data[1 * 4 + 2] * a_0323 + m_data[1 * 4 + 3] * a_0223)
                    + m_data[0 * 4 + 2] * ( m_data[1 * 4 + 0] * a_1323 - m_data[1 * 4 + 1] * a_0323 + m_data[1 * 4 + 3] * a_0123)
                    - m_data[0 * 4 + 3] * ( m_data[1 * 4 + 0] * a_1223 - m_data[1 * 4 + 1] * a_0223 + m_data[1 * 4 + 2] * a_0123);

    det = 1 / det;

    return Matrix4<ValueType>({
        det *   ( m_data[1 * 4 + 1] * a_2323 - m_data[1 * 4 + 2] * a_1323 + m_data[1 * 4 + 3] * a_1223 ),
        det * - ( m_data[0 * 4 + 1] * a_2323 - m_data[0 * 4 + 2] * a_1323 + m_data[0 * 4 + 3] * a_1223 ),
        det *   ( m_data[0 * 4 + 1] * a_2313 - m_data[0 * 4 + 2] * a_1313 + m_data[0 * 4 + 3] * a_1213 ),
        det * - ( m_data[0 * 4 + 1] * a_2312 - m_data[0 * 4 + 2] * a_1312 + m_data[0 * 4 + 3] * a_1212 ),
        det * - ( m_data[1 * 4 + 0] * a_2323 - m_data[1 * 4 + 2] * a_0323 + m_data[1 * 4 + 3] * a_0223 ),
        det *   ( m_data[0 * 4 + 0] * a_2323 - m_data[0 * 4 + 2] * a_0323 + m_data[0 * 4 + 3] * a_0223 ),
        det * - ( m_data[0 * 4 + 0] * a_2313 - m_data[0 * 4 + 2] * a_0313 + m_data[0 * 4 + 3] * a_0213 ),
        det *   ( m_data[0 * 4 + 0] * a_2312 - m_data[0 * 4 + 2] * a_0312 + m_data[0 * 4 + 3] * a_0212 ),
        det *   ( m_data[1 * 4 + 0] * a_1323 - m_data[1 * 4 + 1] * a_0323 + m_data[1 * 4 + 3] * a_0123 ),
        det * - ( m_data[0 * 4 + 0] * a_1323 - m_data[0 * 4 + 1] * a_0323 + m_data[0 * 4 + 3] * a_0123 ),
        det *   ( m_data[0 * 4 + 0] * a_1313 - m_data[0 * 4 + 1] * a_0313 + m_data[0 * 4 + 3] * a_0113 ),
        det * - ( m_data[0 * 4 + 0] * a_1312 - m_data[0 * 4 + 1] * a_0312 + m_data[0 * 4 + 3] * a_0112 ),
        det * - ( m_data[1 * 4 + 0] * a_1223 - m_data[1 * 4 + 1] * a_0223 + m_data[1 * 4 + 2] * a_0123 ),
        det *   ( m_data[0 * 4 + 0] * a_1223 - m_data[0 * 4 + 1] * a_0223 + m_data[0 * 4 + 2] * a_0123 ),
        det * - ( m_data[0 * 4 + 0] * a_1213 - m_data[0 * 4 + 1] * a_0213 + m_data[0 * 4 + 2] * a_0113 ),
        det *   ( m_data[0 * 4 + 0] * a_1212 - m_data[0 * 4 + 1] * a_0212 + m_data[0 * 4 + 2] * a_0112 ),
    });
}

typedef Matrix4<double> Matrix4d;
typedef Matrix4<float> Matrix4f;

#undef SCALE_MATRIX
#undef TRANSLATION_MATRIX
#undef ROT_X_MATRIX
#undef ROT_Y_MATRIX
#undef ROT_Z_MATRIX

#include "engine/utility/matrix2.h"
#include <cmath>

Mat2::Mat2() {
    Identity();
}
void Mat2::Identity() {
    mat[0] = 1.f;
    mat[1] = 0.f;
    mat[2] = 0.f;
    mat[3] = 1.f;
}
void Mat2::Scale(float x, float y) {
    for (int i = 0; i < 2; i++) {
        mat[0 + i] *= x;
        mat[2 + i] *= y;
    }
}
void Mat2::Rotate(float angle) {
    float cosA = std::cos(angle);
    float sinA = std::sin(angle);

    Mat2 o;
    o.mat[0] = cosA; o.mat[2] = -sinA;
    o.mat[1] = sinA; o.mat[3] = cosA;

    *this *= o;
}
Vec2 Mat2::operator*(const Vec2& o) const {
    Vec2 v;

    v.x = mat[0] * o.x + mat[2] * o.y;
    v.y = mat[1] * o.x + mat[3] * o.y;

    return v;
}
Mat2 Mat2::operator*(const Mat2& o) const {
    Mat2 m;

    m.mat[0] = mat[0]*o.mat[0] + mat[2]*o.mat[1];
    m.mat[1] = mat[1]*o.mat[0] + mat[3]*o.mat[1];
    m.mat[2] = mat[0]*o.mat[2] + mat[2]*o.mat[3];
    m.mat[3] = mat[1]*o.mat[2] + mat[3]*o.mat[3];

    return m;
}
Mat2& Mat2::operator*=(const Mat2& o) {
    Mat2 m = *this * o;
    *this = m;
    return *this;
}
Mat2& Mat2::operator=(const Mat2& o) {
    mat[0] = o.mat[0];
    mat[1] = o.mat[1];
    mat[2] = o.mat[2];
    mat[3] = o.mat[3];

    return *this;
}
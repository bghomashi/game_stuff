#include "utility/matrix4.h"
#include <cmath>

Mat4::Mat4() {
    Identity();
}
void Mat4::Identity() {
    for (int i = 0; i < 16; i++)
        mat[i] = 0;
    mat[0] = mat[5] = mat[10] = mat[15] = 1;
}
void Mat4::Translate(float x, float y, float z) {
    mat[12] += x;
    mat[13] += y;
    mat[14] += z; 
}
void Mat4::Scale(float x, float y, float z) {
    for (int i = 0; i < 4; i++) {
        mat[0 + i*4] *= x;
        mat[1 + i*4] *= y;
        mat[2 + i*4] *= z; 
    }
}
void Mat4::Rotate(float angle, Vec3 axis) {
    axis.normalize();

    float sinAngle = sinf(angle);
    float cosAngle = cosf(angle);

    Mat4 rotMat;
    float xx = axis.x * axis.x;
    float yy = axis.y * axis.y;
    float zz = axis.z * axis.z;
    float xy = axis.x * axis.y;
    float yz = axis.y * axis.z;
    float zx = axis.z * axis.x;
    float xs = axis.x * sinAngle;
    float ys = axis.y * sinAngle;
    float zs = axis.z * sinAngle;
    float oneMinusCos = 1.0f - cosAngle;

    rotMat.mat[0] = (oneMinusCos * xx) + cosAngle;
    rotMat.mat[1] = (oneMinusCos * xy) - zs;
    rotMat.mat[2] = (oneMinusCos * zx) + ys;
    rotMat.mat[3] = 0.0F; 

    rotMat.mat[4] = (oneMinusCos * xy) + zs;
    rotMat.mat[5] = (oneMinusCos * yy) + cosAngle;
    rotMat.mat[6] = (oneMinusCos * yz) - xs;
    rotMat.mat[7] = 0.0F;

    rotMat.mat[8] = (oneMinusCos * zx) - ys;
    rotMat.mat[9] = (oneMinusCos * yz) + xs;
    rotMat.mat[10] = (oneMinusCos * zz) + cosAngle;
    rotMat.mat[11] = 0.0F; 

    rotMat.mat[12] = 0.0F;
    rotMat.mat[13] = 0.0F;
    rotMat.mat[14] = 0.0F;
    rotMat.mat[15] = 1.0F;

    *this *= rotMat;
}
const float* Mat4::get() const {
	return mat;
}
float* Mat4::get() {
	return mat;
}
void Mat4::Ortho(float left, float right, float bottom, float top, float near, float far) {
    for (int i = 0; i < 16; i++)
        mat[i] = 0;
    mat[0] = 2.0f / (right - left);
    mat[5] = 2.0f / (top - bottom);
    mat[10] = -2.0f / (far - near);

    mat[12] = -(right + left) / (right - left);
    mat[13] = -(top + bottom) / (top - bottom);
    mat[14] = -(far + near) / (far - near);
    mat[15] = 1.0f;
}
void Mat4::Perspective(float fovy, float aspect, float near, float far) {
	float F = 1.0f / tan(fovy / 2.0f);

	for (int i = 0; i < 16; i++)
		mat[i] = 0;

	mat[0] = F / aspect;
	mat[5] = F;
	mat[10] = (near + far) / (near - far);
	mat[11] = -1;
	mat[14] = 2*far*near / (near - far);
}
Vec2 Mat4::operator*(const Vec2& o) const {
    Vec2 v;

    v.x = mat[0] * o.x + mat[4] * o.y + mat[12];
    v.y = mat[1] * o.x + mat[5] * o.y + mat[13];

    return v;
}
Mat4 Mat4::operator*(const Mat4& o) const 
{
    Mat4 out;

    for (int i = 0; i < 4; i++) {			// for each row	in this
        for (int j = 0; j < 4; j++) {		// for each column in o
            float num = 0;
            for (int k = 0; k < 4; k++) {	// sum over elements in colum
                num += mat[i+k*4] * o.mat[k+j*4];
            }
            out.mat[i+j*4] = num;
        }
    }

    return out;
}
Mat4& Mat4::operator*=(const Mat4& o) {
    Mat4 out;

    for (int i = 0; i < 4; i++) {			// for each row	in this
        for (int j = 0; j < 4; j++) {		// for each column in o
            float num = 0;
            for (int k = 0; k < 4; k++) {	// sum over elements in colum
                num += mat[i+k*4] * o.mat[k+j*4];
            }
            out.mat[i+j*4] = num;
        }
    }

    *this = out;

    return *this;
}
Mat4& Mat4::operator=(const Mat4& o) {
    for (int i = 0; i < 16; i++) 
        mat[i] = o.mat[i];
    return *this;
}
void Mat4::LookAt(Vec3 position, Vec3 center, Vec3 up) {
    Vec3 forward, side;
    // --------------------
    forward = (center - position);
    forward.normalize();
    // --------------------
    // Side = forward x up
    side = (forward.cross(up));
    side.normalize();
    // --------------------
    // Recompute up as: up = side x forward
    up = side.cross(forward);
    // --------------------
    mat[0] = side.x;
    mat[4] = side.y;
    mat[8] = side.z;
    mat[12] = 0.0;
    // --------------------
    mat[1] = up.x;
    mat[5] = up.y;
    mat[9] = up.z;
    mat[13] = 0.0;
    // --------------------
    mat[2] = -forward.x;
    mat[6] = -forward.y;
    mat[10] = -forward.z;
    mat[14] = 0.0;
    // --------------------
    mat[3] = mat[7] = mat[11] = 0.0;
    mat[15] = 1.0;
    // --------------------
    Translate(-position.x, -position.y, -position.z);
    // -----------
}
#include <iostream>

void Mat4::Print() const {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            std::cout << mat[i + j * 4] << " ";
        }
        std::cout << "\n";
    }
}
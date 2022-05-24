#ifndef __MATRIX4_H__
#define __MATRIX4_H__

#include "engine/utility/vector3.h"
#include "engine/utility/vector2.h"

class Mat4 {
    float mat[16];
public:
    Mat4();
    void Identity();
    void Translate(float x, float y, float z);
    void Scale(float x, float y, float z);
    void Rotate(float angle, Vec3 axis);
    void Ortho(float left, float right, float bottom, float top, float near = -1.0, float far = 1.0);
	void Perspective(float fovy, float aspect, float near, float far);
	void LookAt(Vec3 position, Vec3 center, Vec3 up);
	const float* get() const;
	float* get();
    Vec2 operator*(const Vec2& o) const;
    Mat4 operator*(const Mat4& o) const;
    Mat4& operator*=(const Mat4& o);
    Mat4& operator=(const Mat4& o);
    // const float& operator(unsigned row, unsigned col) const;
    // float& operator(unsigned row, unsigned col);
    void Print() const;
};

#endif
#ifndef __MATRIX2_H__
#define __MATRIX2_H__

#include "engine/utility/vector2.h"


class Mat2 {
	float mat[4];
public:
	Mat2();
	void Identity();
	void Scale(float x, float y);
	void Rotate(float angle);
	Vec2 operator*(const Vec2& o) const;
	Mat2 operator*(const Mat2& o) const;
	Mat2& operator*=(const Mat2& o);
	Mat2& operator=(const Mat2& o);
};


#endif
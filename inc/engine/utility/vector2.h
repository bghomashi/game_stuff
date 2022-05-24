#ifndef __VECTOR2_H__
#define __VECTOR2_H__

#include <cmath>

template <typename T>
struct TVec2 {
	T x, y;


	inline float lengthSqr() const {
		return x * x + y * y;
	}
	inline float length() const {
		return std::sqrt(x * x + y * y);
	}
	inline void normalize() {
		float l = length();
		x /= l;
		y /= l;
	}
	inline const TVec2 norm() const {
		float l = length();
		if (l == 0) return { 0,0 };
		return *this / l;
	}
	inline float dot(const TVec2& o) const {
		return x * o.x + y * o.y;
	}
	inline void negate() {
		x = -x;
		y = -y;
	}
	inline TVec2 negative() const {
		TVec2 res = *this;
		res.negate();
		return res;
	}
	inline TVec2 sign() const {
		TVec2 res = *this;
		
		if (res.x > 0) res.x = 1.0f;
		else if (res.x < 0) res.x = -1.0f;
		else res.x = 0;

		if (res.y > 0) res.y = 1.0f;
		else if (res.y < 0) res.y = -1.0f;
		else res.y = 0;

		return res;
	}
	inline float cross(const TVec2& o) const {
		return x * o.y - y * o.x;
	}

	inline TVec2 operator-() const {
		TVec2 res = { -x, -y };
		return res;
	}
	inline TVec2& operator=(const TVec2& o) {
		x = o.x;
		y = o.y;
		return *this;
	}
	inline TVec2& operator+=(const TVec2& o) {
		x += o.x;
		y += o.y;
		return *this;
	}
	inline TVec2& operator-=(const TVec2& o) {
		x -= o.x;
		y -= o.y;
		return *this;
	}
	TVec2& operator*=(const TVec2& o) {
		x *= o.x;
		y *= o.y;
		return *this;
	}
	TVec2& operator/=(const TVec2& o) {
		x /= o.x;
		y /= o.y;
		return *this;
	}
	TVec2 operator*(const TVec2& o) const {
		TVec2 res = *this;
		res *= o;
		return res;

	}
	TVec2 operator/(const TVec2& o) const {
		TVec2 res = *this;
		res /= o;
		return res;
	}
	inline TVec2 operator+(const TVec2& o) const {
		TVec2 res = *this;
		res += o;
		return res;
	}
	inline TVec2 operator-(const TVec2& o) const {
		TVec2 res = *this;
		res -= o;
		return res;
	}
	inline TVec2& operator*=(const float o) {
		x *= o;
		y *= o;
		return *this;
	}
	inline TVec2& operator/=(const float o) {
		x /= o;
		y /= o;
		return *this;
	}
	inline TVec2 operator*(const T o) const {
		TVec2 res = *this;
		res *= o;
		return res;
	}
	inline TVec2 operator/(const T o) const {
		TVec2 res = *this;
		res /= o;
		return res;
	}

	inline bool operator==(const TVec2& o) const {
		return x == o.x && y == o.y;
	}
	inline bool operator==(const TVec2& o) const {
		return x != o.x || y != o.y;
	}

	TVec2<float> ToFloats() const {
		return TVec2<float>{(float)x, (float)y};
	}

	TVec2<int> ToInts() const {
		return TVec2<int>{(int)x, (int)y};
	}
};


template <typename T>
inline TVec2<float> operator*(const float o2, const TVec2<T>& o1) {
	TVec2<float> res = o1.ToFloats();
	res *= o2;
	return res;
}
template <typename T>
inline TVec2<float> operator/(const float o2, const TVec2<T>& o1) {
	TVec2<float> res = o1.ToFloats();
	res /= o2;
	return res;
}
typedef TVec2<float> Vec2;
typedef TVec2<int> iVec2;
typedef TVec2<double> dVec2;

#endif
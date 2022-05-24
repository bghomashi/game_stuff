#ifndef __VECTOR3_H__
#define __VECTOR3_H__

#include <cmath>

template <typename T>
class TVec3 {
public:
	float x, y, z;

	float lengthSqr() const {
		return x * x + y * y + z * z;
	}
	float length() const {
		return std::sqrt(x * x + y * y + z * z);
	}

	const TVec3 norm() const {
		float l = length();
		if (l == 0) return { 0,0 };
		return *this / l;
	}
	TVec3 cross(const TVec3& o) const {
		TVec3 res;
		res.x = y * o.z - z * o.y;
		res.y = z * o.x - x * o.z;
		res.z = x * o.y - y * o.x;
		return res;
	}
	float dot(const TVec3& o) const {
		return x * o.x + y * o.y + z * o.z;
	}

	void negate() {
		x = -x;
		y = -y;
	}
	void normalize() {
		float l = std::sqrt(x * x + y * y + z * z);
		x = x / l;
		y = y / l;
		z = z / l;
	}

	TVec3 operator+(const float o) const {
		TVec3 res;
		res.x = x + o;
		res.y = y + o;
		res.z = z + o;
		return res;
	}
	TVec3 operator-(const float o) const {
		TVec3 res;
		res.x = x - o;
		res.y = y - o;
		res.z = z - o;
		return res;
	}
	TVec3 operator*(const float o) const {
		TVec3 res;
		res.x = x * o;
		res.y = y * o;
		res.z = z * o;
		return res;
	}
	TVec3 operator/(const float o) const {
		TVec3 res;
		res.x = x / o;
		res.y = y / o;
		res.z = z / o;
		return res;
	}
	TVec3 operator+(const TVec3& o) const {
		TVec3 res;
		res.x = x + o.x;
		res.y = y + o.y;
		res.z = z + o.z;
		return res;
	}
	TVec3 operator-(const TVec3& o) const {
		TVec3 res;
		res.x = x - o.x;
		res.y = y - o.y;
		res.z = z - o.z;
		return res;
	}
	TVec3& operator*=(const float o) {
		x *= o;
		y *= o;
		z *= o;
		return *this;
	}
	TVec3& operator/=(const float o) {
		x /= o;
		y /= o;
		z /= o;
		return *this;
	}
	TVec3& operator+=(const TVec3& o) {
		x += o.x;
		y -= o.y;
		z -= o.z;
		return *this;
	}
	TVec3& operator-=(const TVec3& o) {
		x -= o.x;
		y -= o.y;
		z -= o.z;
		return *this;
	}
	TVec3 operator=(const TVec3& o) {
		x = o.x;
		y = o.y;
		z = o.z;
		return *this;
	}

	bool operator==(const TVec3& o) const {
		return x == o.x && y == o.y && z == o.z;
	}
};

typedef TVec3<float> Vec3;
typedef TVec3<int> iVec3;

#endif
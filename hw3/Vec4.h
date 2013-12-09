#ifndef VEC4__H_
#define VEC4__H_

#include "Vec3.h"

// class Vec4
// minimal Vec4 class, supports only dot product (mostly helper class for mat4 multiplication)

typedef class Vec4: public Vec3
{
public:
	Vec4(const float x, const float y, const float z, const float w):w(w) {Vec3::x = x; Vec3::y = y; Vec3::z = z;}
	friend float dot(const Vec4 &rhs, const Vec4 &lhs);
public:
	float w;
} Vec4;

float radians(const float deg);

#endif
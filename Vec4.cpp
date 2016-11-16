#include "Vec4.h"

// radians()
// params: angle measure in degrees
// return: angle measure in rad
// notes:

float radians(const float deg)
{
	return deg * 3.14159 / 180;
}

// dot()
// params: vec4s to dot product
// return: float dot product of vectors
// notes:

float dot(const Vec4 &rhs, const Vec4 &lhs)
{
	return rhs.x * lhs.x + rhs.y * lhs.y + rhs.z * lhs.z + rhs.w * lhs.w;
}

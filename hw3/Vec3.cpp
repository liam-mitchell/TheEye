#include "Vec3.h"
#include <math.h>
#include "Point.h"

Vec3::Vec3(const Point &rhs):x(rhs.x), y(rhs.y), z(rhs.z) {}

Vec3 &Vec3::operator+=(const Vec3 &rhs)
{
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;

	return *this;
}

Vec3 Vec3::operator+(const Vec3 &rhs) const
{
	Vec3 temp = *this;
	temp += rhs;
	return temp;
}

Vec3 &Vec3::operator-=(const Vec3 &rhs)
{
	x -= rhs.x;
	y -= rhs.y;
	z -= rhs.z;

	return *this;
}

Vec3 Vec3::operator-(const Vec3 &rhs) const
{
	Vec3 temp = *this;
	temp -= rhs;
	return temp;
}

Vec3 &Vec3::operator*=(const float &rhs)
{
	x *= rhs;
	y *= rhs;
	z *= rhs;

	return *this;
}

Vec3 Vec3::operator*(const float &rhs) const
{
	Vec3 temp = *this;
	temp *= rhs;
	return temp;
}

Vec3 &Vec3::operator/=(const float &rhs)
{
	if (rhs != 0)
	{
		x /= rhs;
		y /= rhs;
		z /= rhs;
	}

	return *this;
}

Vec3 Vec3::operator/(const float &rhs) const
{
	Vec3 temp = *this;
	temp /= rhs;
	return temp;
}

// normal()
// params: 
// return: vector with direction of this, of unit length
// notes:

Vec3 Vec3::normal() const
{
	if (!(x == 0 && y == 0 && z == 0))
	{
		float length = sqrt(x * x + y * y + z * z);
		Vec3 temp = *this;
		temp /= length;
		return temp;
	} else return *this;
}

// length()
// params:
// return: float length of vector
// notes:

float Vec3::length() const
{
	return sqrt(x * x + y * y + z * z);
}

// dot()
// params: vec3s to dot product
// return: float dot product of two vec3s
// notes:

float dot(const Vec3 &lhs, const Vec3 &rhs)
{
	return rhs.x * lhs.x + rhs.y * lhs.y + rhs.z * lhs.z;
}

// cross()
// params: vec3s to cross product
// return: vec3 cross product of two vec3s
// notes:

Vec3 cross(const Vec3 &lhs, const Vec3 &rhs)
{
	float x = (lhs.y * rhs.z - lhs.z * rhs.y);
	float y = (lhs.z * rhs.x - lhs.x * rhs.z);
	float z = (lhs.x * rhs.y - lhs.y * rhs.x);

	return Vec3(x, y, z);
}
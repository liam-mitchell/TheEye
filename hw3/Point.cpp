#include "Point.h"

Vec3 Point::operator-(const Point &rhs) const //note: returns vec3 difference between the two points - it's not possible to add two points
{
	Vec3 temp(x - rhs.x, y - rhs.y, z - rhs.z);
	return temp;
}

Point &Point::operator+=(const Vec3 &rhs) //adds a vector onto a point
{
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;

	return *this;
}

Point Point::operator+(const Vec3 &rhs) const
{
	Point temp = *this;
	temp += rhs;
	return temp;
}

Point &Point::operator-=(const Vec3 &rhs)
{
	x -= rhs.x;
	y -= rhs.y;
	z -= rhs.z;

	return *this;
}

Point Point::operator-(const Vec3 &rhs) const
{
	Point temp = *this;
	temp -= rhs;
	return temp;
}
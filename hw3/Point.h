#ifndef POINT__H_
#define POINT__H_

#include "Vec3.h"

// class Point
// simple 3d point class

typedef class Point
{
public:
	Point():x(0), y(0), z(0) {}
	Point(const Vec3 &src):x(src.x), y(src.y), z(src.z) {}
	Point(const float x, const float y, const float z):x(x), y(y), z(z) {}
	Vec3 operator-(const Point &rhs) const;
	Point &operator+=(const Vec3 &rhs);
	Point operator+(const Vec3 &rhs) const;
	Point &operator-=(const Vec3 &rhs);
	Point operator-(const Vec3 &rhs) const;
public:
	float x, y, z;
} Point;

#endif
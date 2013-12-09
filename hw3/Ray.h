#ifndef RAY__H_
#define RAY__H_

#include "Vec3.h"
#include "Point.h"
#include <limits>

// class Ray
// represents a light ray, with origin, direction, and min and max distance

typedef class Ray
{
public:
	Ray():origin(), direction(), tmin(0), tmax(std::numeric_limits<float>::max()) {}
	Ray(const Point o, const Vec3 d, const float tmin = 0, const float tmax = std::numeric_limits<float>::max()): origin(o), direction(d), tmin(tmin), tmax(tmax) {} // defaults tmin and tmax to zero and very large respectively
public:
	Point origin;
	Vec3 direction;
	float tmin, tmax;
} Ray;

#endif
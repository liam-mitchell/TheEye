#ifndef INTERSECTION__H_
#define INTERSECTION__H_

#include "Point.h"

// class Intersection
// plain old data structure for ray/surface intersection data

class Primitive;

typedef class Intersection
{
public:
	Point hit;
	Vec3 normal;
	Primitive *object;
} Intersection;

#endif
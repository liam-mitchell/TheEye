#ifndef PRIMITIVE__H_
#define PRIMITIVE__H_

#include "Ray.h"
#include "Intersection.h"
#include "Color.h"

// class Primitive
// abstract interface for all shapes
// provides methods for returning lighting materials of object, as well as intersecting with a ray

class Primitive
{
public:
	virtual Color color() = 0;
	virtual Color diffuse() = 0;
	virtual Color specular() = 0;
	virtual Color emission() = 0;
	virtual float shininess() = 0;
	virtual bool intersect(const Ray &ray, float *t, Intersection *intersect) = 0;
	virtual ~Primitive() {}
};

#endif
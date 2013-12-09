#ifndef LIGHT__H_
#define LIGHT__H_

#include "Vec3.h"
#include <list>
#include <memory>

// class Light
// abstract interface to be implemented by different types of light
// provides methods for getting light strength, color and direction, as well as creation and intersection of a shadow ray from point

const float EPSILON = .01f;

class Point;
class Primitive;

class Light
{
public:
	virtual Vec3 color() = 0;
	virtual Vec3 vector(const Point &rhs) = 0;
	virtual float attenuation(const Point &location) = 0;
	virtual bool intersectShadowRay(const Point &source, const std::list<std::unique_ptr<Primitive> > &primitives) = 0;
	virtual ~Light() {}
};

#endif
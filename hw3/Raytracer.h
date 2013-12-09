#ifndef RAYTRACER__H_
#define RAYTRACER__H_

#include "Color.h"
#include "Ray.h"
#include <list>
#include <memory>

// class Raytracer
// responsible for tracing rays through objects and lights in a scene, aggregating colors and returning overall colors of rays

class Primitive;
class Light;
class Intersection;
class Camera;

typedef class Raytracer
{
public:
	Color trace(const Ray &ray, const std::list<std::unique_ptr<Primitive> > &primitives, const std::list<std::unique_ptr<Light> > &lights, const float depth = 0); // defaults recursion depth to zero - calls itself with depth = depth + 1 inside
private:
	Color computeColor(const std::list<std::unique_ptr<Light> > &lights, const std::list<std::unique_ptr<Primitive> > &primitives, const Intersection &intersect, const Ray &ray); // computes color based on object hit and shading from all lights in the scene
	float m_mindist;
} Raytracer;

#endif
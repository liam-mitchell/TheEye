#include "Raytracer.h"
#include "Camera.h"
#include <limits>
#include <algorithm>
#include "Intersection.h"
#include "Primitive.h"
#include "Light.h"

// trace()
// params: ray to trace, primitives and lights to intersect and shade with, recursion depth (defaults to zero - called recursively up to depth of 5)
// return: color of traced ray
// notes:

Color Raytracer::trace(const Ray &ray, const std::list<std::unique_ptr<Primitive> > &primitives, const std::list<std::unique_ptr<Light> > &lights, const float depth)
{
	if (depth > 5) return Color(0, 0, 0);

	m_mindist = std::numeric_limits<float>::max();

	Intersection intersect, current_intersect;
	intersect.object = nullptr;
	float current_t;

	for (auto it = primitives.begin(); it != primitives.end(); ++it) // find closest intersection with primitives
	{
		if ( (*it)->intersect(ray, &current_t, &current_intersect) )
		{
			if (current_t < m_mindist)
			{
				m_mindist = current_t;
				intersect = current_intersect;
			}
		}
	}

	if (intersect.object) //if we found an intersection, color with that object's color and shade with lights
	{
		Color ret = computeColor(lights, primitives, intersect, ray);

		Vec3 indir = Vec3(0, 0, 0) - ray.direction; //calculate reflected ray direction (angle in = angle out)
		Vec3 outdir = intersect.normal * dot(indir, intersect.normal) * 2 - indir;
		Ray secondaryRay(intersect.hit, outdir, EPSILON, std::numeric_limits<float>::max());

		ret += intersect.object->specular() * trace(secondaryRay, primitives, lights, depth + 1); //compute specular reflection
		return ret.clamp();
	}
	else
	{
		Color ret(0, 0, 0); //if we don't find anything color black
		return ret;
	}

}

// computeColor()
// params: lights and primitives to intersect shadow rays with, intersection point, original ray to intersect
// return: color of ray
// notes: shades lights using the Phong model, after figuring out whether it's shaded or not

Color Raytracer::computeColor(const std::list<std::unique_ptr<Light> > &lights, const std::list<std::unique_ptr<Primitive> > &primitives, const Intersection &intersect, const Ray &ray)
{
	Vec3 eye_direction = (ray.origin - intersect.hit).normal();

	Color color(intersect.object->color() + intersect.object->emission());

	for (auto it = lights.begin(); it != lights.end(); ++it)
	{
		if ( (*it)->intersectShadowRay(intersect.hit, primitives) ) continue; //don't shade this light if it's shadowed

		float nDotL = dot(intersect.normal, (*it)->vector(intersect.hit).normal()); //compute diffuse and specular highlights
		Color lambert = intersect.object->diffuse() * (*it)->color() * std::max(nDotL, 0.0f);

		Vec3 halfvec = (eye_direction + (*it)->vector(intersect.hit).normal()).normal();
		float nDotH = dot(intersect.normal, halfvec);
		Color phong = intersect.object->specular() * (*it)->color() * pow( std::max(nDotH, 0.0f), intersect.object->shininess());

		color += (lambert + phong) * (*it)->attenuation(intersect.hit); //scale by attenuation of light
	}

	return color;
}
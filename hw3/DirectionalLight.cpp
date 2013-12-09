#include "DirectionalLight.h"
#include "Ray.h"
#include "Intersection.h"
#include "Primitive.h"

// vector()
// params: dummy point (to fill reqs of light::vector)
// return: direction vector of light
// notes: directional lights don't care where we are, they illuminate evenly everywhere - point simply satisfies the requirements for implementing light::vector()

Vec3 DirectionalLight::vector(const Point &source) 
{
	return (m_direction).normal();
}

// intersectShadowRay()
// params: start point of ray, list of primitives to intersect with
// return: bool visibility of light from point
// notes: implements light::intersectShadowRay()

bool DirectionalLight::intersectShadowRay(const Point &source, const std::list<std::unique_ptr<Primitive> > &primitives)
{
	Vec3 raydir = (m_direction).normal();
	Ray shadowray(source, raydir, EPSILON);

	float t;
	Intersection i;

	for (auto it = primitives.begin(); it != primitives.end(); ++it)
	{
		if ( (*it)->intersect(shadowray, &t, &i) )
		{
			return true;
		}
	}

	return false;
}
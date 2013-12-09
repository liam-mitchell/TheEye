#include "PointLight.h"
#include "Ray.h"
#include "Intersection.h"
#include "Primitive.h"

// vector()
// params: point to evaluate light direction at
// return: vec3 from source to light
// notes: implements light::vector()

Vec3 PointLight::vector(const Point &src)
{
	return (m_position - src).normal();
}

// attenuation()
// params: point to evaluate attenuation of light at
// return: float representing light falloff % at point
// notes: implements light::attenuation()

float PointLight::attenuation(const Point &src)
{
	float d = (m_position - src).length();
	return 1.0f / (m_attenuation.x + m_attenuation.y * d + m_attenuation.z * (d * d));
}

// intersectShadowRay()
// params: source point of shadow ray, list of primitives to intersect with
// return: bool visibility of light from point
// notes: implements light::intersectShadowRay()

bool PointLight::intersectShadowRay(const Point &src, const std::list<std::unique_ptr<Primitive> > &primitives)
{
	float maxdist = (m_position - src).length();
	Vec3 direction = (m_position - src).normal();

	float t;
	Intersection i;

	Ray shadowray(src, direction, EPSILON, maxdist);

	for (auto it = primitives.begin(); it != primitives.end(); ++it)
	{
		if ( (*it)->intersect(shadowray, &t, &i) ) return true;
	}

	return false;
}
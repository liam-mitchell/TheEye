#include "Sphere.h"
#include <math.h>
#include <algorithm>

// intersect()
// params: ray to intersect with, float and intersection to store intersect data in (if found)
// return: bool did the ray intersect the sphere
// notes: implements primitive::intersect()

bool Sphere::intersect(const Ray &ray, float *t, Intersection *intersect)
{
	Vec3 ray_origin(ray.origin.x, ray.origin.y, ray.origin.z);
	Vec3 ray_direction(ray.direction.normal());
	Vec3 sphere_center(m_center.x, m_center.y, m_center.z);

	float a = dot(ray_direction, ray_direction); //solution of the quadratic derived from (P - C) . (P - C) - r^2 = 0 (sphere equation) with P = ray_origin + t * ray direction
	float b = 2 * dot(ray_direction, ray_origin - sphere_center);
	float c = dot(ray_origin - sphere_center, ray_origin - sphere_center) - m_radius * m_radius;

	float discriminant = b * b - 4 * a * c;
	if (discriminant < 0) //no intersection - complex roots
	{
		return false;
	}
	else
	{
		float r1 = (-b + sqrt(discriminant) ) / (2 * a); //yes intersections
		float r2 = (-b - sqrt(discriminant) ) / (2 * a);
		
		if (r1 < ray.tmin || r1 > ray.tmax) //disallow line intersections outside ray bounds
		{
			if (r2 < ray.tmin || r2 > ray.tmax) return false;
			else *t = r2;
		}
		else if (r2 < ray.tmin || r2 > ray.tmax)
		{
			if (r1 < ray.tmin || r2 < ray.tmax) return false;
			else *t = r1;
		}
		else 
		{
			*t = std::min(r1, r2); //take the closest intersection
		}

		intersect->hit = ray.origin + ray.direction.normal() * (*t); //fill in intersection data
		intersect->normal = (intersect->hit - m_center).normal();
		intersect->object = this;
		return true;
	}
}
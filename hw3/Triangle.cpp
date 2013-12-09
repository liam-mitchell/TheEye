#include "Triangle.h"

Triangle::Triangle(const Point &v1, const Point &v2, const Point &v3, const Color &color, const Color &diffuse, const Color &specular, const Color &emission, const float shininess):m_v1(v1), m_v2(v2), m_v3(v3), m_color(color), m_diffuse(diffuse), m_specular(specular), m_emission(emission), m_shininess(shininess)
{
	m_normal = cross( v2 - v1, v3 - v1 ).normal();
}

// intersect()
// params: ray to intersect with, float and intersect to store intersection data (if found)
// return: bool did the ray hit the tri
// notes: uses the inside-out test (can be generalized for more complex polygons)

bool Triangle::intersect(const Ray &ray, float *t, Intersection *intersect)
{
	Vec3 ray_origin = ray.origin;
	Vec3 v1 = m_v1;
	Vec3 v2 = m_v2;
	Vec3 v3 = m_v3;

	if (dot(ray.direction, m_normal) == 0) return false; //ray is parallel to the plane - no intersection

	float plane_intersect_dist = ( dot(v1, m_normal) - dot(ray_origin, m_normal) ) / dot(ray.direction, m_normal); //otherwise we have an intersection

	if (plane_intersect_dist <= ray.tmin || plane_intersect_dist > ray.tmax) return false; //if we don't hit the plane within the ray bounds

	Vec3 planehit = ray.origin + ray.direction * plane_intersect_dist; //point where we hit the plane

	Vec3 v1v2 = v2 - v1; //inside-out test
	Vec3 v1p = planehit - v1;
	float u = dot(m_normal, cross(v1v2, v1p));
	if (u < 0) return false;

	Vec3 v2v3 = v3 - v2;
	Vec3 v2p = planehit - v2;
	float v = dot(m_normal, cross(v2v3, v2p));
	if (v < 0) return false;

	Vec3 v3v1 = v1 - v3;
	Vec3 v3p = planehit - v3;
	float w = dot(m_normal, cross(v3v1, v3p));
	if (w < 0) return false;

	*t = plane_intersect_dist;	//fill intersection data
	intersect->hit = planehit;
	intersect->normal = m_normal;
	intersect->object = this;
	return true;
}
#include "Ellipse.h"
#include "Intersection.h"

Ellipse::Ellipse(const Mat4 &rot, const Mat4 &trans, const Mat4 &scale, const Mat4 &inv_rot, const Mat4 &inv_trans, const Mat4 &inv_scale, const Color &color, const Color &diffuse, const Color &specular, const Color &emission, const float shininess):rot(rot), trans(trans), scale(scale), inv_rot(inv_rot), inv_trans(inv_trans), inv_scale(inv_scale) 
{
	m_color = color;
	m_diffuse = diffuse;
	m_specular = specular;
	m_emission = emission;
	m_shininess = shininess;
}

// intersect()
// params: ray to intersect with, float and intersection pointers to store intersect data in (if it occurs)
// return: bool does the ray hit this ellipse
// notes: transforms ray by inverse of the transform from the unit sphere centered at origin, then collides transformed ray with unit sphere
//		  same as colliding untransformed ray with transformed sphere

bool Ellipse::intersect(const Ray &ray, float *t, Intersection *intersect)
{
	Ray transformray = ray;

	transformray.origin = inv_scale * inv_rot * inv_trans * transformray.origin; //origin is fully invese-transformed in opposite order of application
	transformray.direction = inv_scale * inv_rot * transformray.direction; //direction is a vector - isn't affected by translations, so don't invert translation

	Sphere s;

	if (s.intersect(transformray, t, intersect)) //collide transformed ray with unit sphere
	{
		intersect->hit = trans * rot * scale * intersect->hit; //retransform hit point back into normal world coordinates
		*t = (intersect->hit - ray.origin).length() / ray.direction.length();

		intersect->object = this;
		intersect->normal = (rot * inv_scale * intersect->normal).normal(); //transform normal (again no translation, it's a vector - and normalize afterwards)
		return true;
	}

	else return false;
}
#ifndef SPHERE__H_
#define SPHERE__H_

#include "Primitive.h"
#include "Point.h"
#include "Color.h"

// class Sphere
// implements abstract interface Primitive
// implements simple sphere-ray intersection, as well as methods for returning material properties of sphere

typedef class Sphere: public Primitive
{
public:
	Sphere():m_radius(1), m_center(), m_color(), m_diffuse(), m_specular(), m_emission(), m_shininess(1.0f) {}
	Sphere(const float r, const Point &c, const Color &col, const Color &diffuse, const Color &specular, const Color &emission, const float &shininess):m_radius(r), m_center(c), m_color(col), m_diffuse(diffuse), m_specular(specular), m_emission(emission), m_shininess(shininess) {}
	virtual bool intersect(const Ray &ray, float *t, Intersection *intersect) override;
	virtual Color color() override {return m_color;}
	virtual Color diffuse() override {return m_diffuse;}
	virtual Color specular() override {return m_specular;}
	virtual Color emission() override {return m_emission;}
	virtual float shininess() override {return m_shininess;}

	virtual ~Sphere() {}
protected:
	float m_radius;
	Point m_center;

	Color m_color, m_diffuse, m_specular, m_emission; //lighting properties
	float m_shininess;
} Sphere;

#endif
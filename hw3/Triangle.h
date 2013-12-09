#ifndef TRIANGLE__H_
#define TRIANGLE__H_

// class Triangle
// implements interface Primitive
// uses the inside-out test to implement intersection method
// as well as implementing material properties methods

#include "Primitive.h"
#include "Point.h"

typedef class Triangle: public Primitive
{
public:
	Triangle(const Point &v1, const Point &v2, const Point &v3, const Color &color, const Color &diffuse, const Color &specular, const Color &emission, const float shininess); 
	virtual Color color() override {return m_color;}
	virtual Color diffuse() override {return m_diffuse;}
	virtual Color specular() override {return m_specular;}
	virtual Color emission() override {return m_emission;}
	virtual float shininess() override {return m_shininess;}
	virtual bool intersect(const Ray &ray, float *t, Intersection *intersect) override;

	virtual ~Triangle() {}
private:
	Point m_v1, m_v2, m_v3;
	Vec3 m_normal;

	Color m_color, m_diffuse, m_specular, m_emission;
	float m_shininess;
} Triangle;

#endif
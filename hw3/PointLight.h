#ifndef POINTLIGHT__H_
#define POINTLIGHT__H_

#include "Light.h"
#include "Point.h"
#include <memory>

// class PointLight
// implements abstract interface Light
// Represents a light at a point in space, emitting evenly in all directions

typedef class PointLight: public Light
{
public:
	PointLight(const Point &position, const Vec3 &color, const Vec3 &atten = Vec3(1, 0, 0)):m_color(color), m_position(position), m_attenuation(atten) {}

	virtual Vec3 color() override {return m_color;} //inherited Light:: methods
	virtual Vec3 vector(const Point &src) override;
	virtual float attenuation(const Point &location) override;
	virtual bool intersectShadowRay(const Point &src, const std::list<std::unique_ptr<Primitive> > &primitives) override;
	virtual ~PointLight() {}
private:
	Point m_position;
	Vec3 m_color;
	Vec3 m_attenuation;

} PointLight;

#endif
#ifndef DIRLIGHT__H_
#define DIRLIGHT__H_

#include "Vec3.h"
#include "Light.h"
#include <memory>

// class DirectionalLight
// implements abstract interface Light
// stores direction and color of light
// attenuation is constant, directional light doesn't even store it
// Represents a very distant light

typedef class DirectionalLight: public Light
{
public:
	DirectionalLight(const Vec3 &direction, const Vec3 &color):m_direction(direction), m_color(color) {}

	virtual Vec3 vector(const Point &source) override; // inherited light:: methods
	virtual Vec3 color() override {return m_color;}
	virtual bool intersectShadowRay(const Point &source, const std::list<std::unique_ptr<Primitive> > &primitives) override;
	virtual float attenuation(const Point &location) override {return 1.0f;}
	virtual ~DirectionalLight() {}
private:
	Vec3 m_direction;
	Vec3 m_color;
} DirectionalLight;


#endif //DIRLIGHT__H_
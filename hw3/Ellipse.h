#ifndef ELLIPSE__H_
#define ELLIPSE__H_

#include "Mat4.h"
#include "Sphere.h"

// class Ellipse
// inherits from Sphere and therefore also implements Primitive interface
// overrides only Sphere's intersect method - stores additional information about ellipse transformations

typedef class Ellipse: public Sphere
{
public:
	Ellipse() {}
	Ellipse(const Mat4 &rot, const Mat4 &trans, const Mat4 &scale, const Mat4 &inv_rot, const Mat4 &inv_trans, const Mat4 &inv_scale, const Color &color, const Color &diffuse, const Color &specular, const Color &emission, const float shininess);
	virtual bool intersect(const Ray &ray, float *t, Intersection *intersect) override;
	virtual ~Ellipse() {}
private:
	Mat4 rot, trans, scale, inv_rot, inv_trans, inv_scale;
} Ellipse;

#endif
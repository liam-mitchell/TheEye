#ifndef CAMERA__H_
#define CAMEA__H_

#include "Vec3.h"
#include "Ray.h"
#include "Sampler.h"

// class Camera
// maintains camera's coordinate axes, position and field of view for use in rendering

typedef class Camera
{
public:
	Camera():m_pos(), m_u(1, 0, 0), m_v(0, 1, 0), m_w(0, 0, 1) {}
	Camera(float posx, float posy, float posz, float lookatx, float lookaty, float lookatz, float upx, float upy, float upz, float fovy);
	Ray generateRay(const Sample &s, const float aspect); //generates a ray through normalized image plane coords

	Point point() const {return m_pos;} //returns the camera's position
private:
	Point m_pos; // position

	Vec3 m_w; // coordinate axes
	Vec3 m_v;
	Vec3 m_u;

	float m_fovy; // field of view in y direction (degrees)
} Camera; 

#endif
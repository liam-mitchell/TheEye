#include "Camera.h"
#include "Mat4.h"
#include <cmath>

// Camera()
// params: 10 floats - 3 vec3s (position, lookat, up) and fovy in degrees
// return: constructs camera with orthogonal and normal coordinate axes
// notes: m_w = forward (lookat target), m_v = upvector, m_u = right of camera

Camera::Camera(float posx, float posy, float posz, float lookatx, float lookaty, float lookatz, float upx, float upy, float upz, float fovy):m_pos(posx, posy, posz), m_fovy(fovy)
{
	Vec3 lookat(lookatx, lookaty, lookatz);
	Vec3 w = lookat - m_pos;
	m_w = w.normal();

	Vec3 up(upx, upy, upz);
	m_u = cross(w, up).normal();

	m_v = cross(m_w, m_u);	
}

// generateRay()
// params: sample w/ normalized x and y coords on image plane, aspect ratio of image
// return: ray with origin = camera, direction through sample
// notes: scales normalized x and y coords by tan(fovy / 2) and tan(fovx / 2), then generates ray through new x and y coords * u and v axes, 1 * w axis

Ray Camera::generateRay(const Sample &s, const float aspect)
{
	float y_scale = std::tan(radians(m_fovy) / 2);
	float x_scale = y_scale * aspect;

	float a = x_scale * s.x;
	float b = y_scale * s.y;

	Vec3 dir(m_u * a + m_v * b + m_w);
	
	Ray ret(m_pos, dir);
	return ret;
}

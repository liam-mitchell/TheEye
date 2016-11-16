#include "Camera.h"

// there are so many parameters omg
// param pos: position
// param lookat: lookat point
// param up: camera upvector
// param fovy: camera field of view
// param aperture: camera aperture width for depth of field
// param focallength: camera focal length for depth of field
// param dof: whether or not to render with depth of field
Camera::Camera(float posx, float posy, float posz, float lookatx, float lookaty, float lookatz, float upx, float upy, float upz, float fovy, float aperture, float focallength, bool dof, unsigned int ndepthrays):pos(posx, posy, posz), fovy(fovy), aperture(aperture), focallength(focallength), dof(dof), ndepthrays(ndepthrays)
{
	Vec3 lookat(lookatx, lookaty, lookatz);
	w = (lookat - pos).normal();
	
	Vec3 up(upx, upy, upz);
	u = cross(w, up).normal();

	v = cross(w, u).normal();	
}

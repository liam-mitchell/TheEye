#ifndef CAMERA__H_
#define CAMEA__H_

#include "Vec3.h"

// class Camera
// maintains camera's coordinate axes, position and field of view for use in rendering
struct Camera
{
public:
 	Camera() : pos(), w(0, 0, 1), v(0, 1, 0), u(1, 0, 0), fovy(45), aperture(0), focallength(0), dof(false), ndepthrays(4) {}
	Camera(float posx, float posy, float posz, float lookatx, float lookaty, float lookatz, float upx, float upy, float upz, float fovy, float aperture, float focallength, bool dof, unsigned int ndepthrays);

	Vec3 pos; // position

	Vec3 w; // coordinate axes
	Vec3 v;
	Vec3 u;

	float fovy; // field of view in y direction (degrees)

        float aperture;
        float focallength;
        bool dof;
	unsigned int ndepthrays;
}; 

#endif

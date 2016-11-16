#ifndef MATERIAL_H_
#define MATERIAL_H_

#include "Vec3.h"

struct Material {
	Vec3 ambient;
        float pad1 = 0;

	Vec3 diffuse;
        float pad2 = 0;

	Vec3 specular;
	float pad3 = 0;

	Vec3 emission;
	float shininess;
	
	Material(Vec3 a, Vec3 d, Vec3 spec, Vec3 e, float s) : ambient(a), diffuse(d), specular(spec), emission(e), shininess(s) {}
};

#endif // MATERIAL_H_

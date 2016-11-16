#ifndef PRIMITIVE__H_
#define PRIMITIVE__H_

#include "Material.h"

// Primitive definitions (sphere, triangle, plane).
// Padded carefully to match the struct layouts in raytracer.frag (see their definitions
// there for details).
struct Sphere {
    Material material;
    Vec3 position;
    float radius;

    Sphere(Material m, Vec3 p, float r) : material(m), position(p), radius(r) {}
};

struct Triangle {
    Material material;
    Vec3 v0;
    float pad0 = 0;

    Vec3 v1;
    float pad1 = 0;

    Vec3 v2;
    float pad2 = 0;

    Vec3 normal;
    float pad3 = 0;

    Triangle(Material m, Vec3 v[3]) : material(m), v0(v[0]), v1(v[1]), v2(v[2]), normal(cross(v1 - v0, v2 - v0).normal()) {}
};

struct Plane {
    Material material;

    Vec3 normal;
    float pad0 = 0;

    Vec3 point;
    float pad1 = 0;

    Plane(Material m, Vec3 n, Vec3 p) : material(m), normal(n), point(p) {}
};

#endif

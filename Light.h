#ifndef LIGHT__H_
#define LIGHT__H_

#include "Vec3.h"

// Light structure.
// Padded so as to match the structure layout in GLSL (see comments on the Light struct
// in raytracer.frag for details).
struct Light {
    Vec3 position;
    float pad1 = 0;
    Vec3 attenuation;
    float pad2 = 0;
    Vec3 colour;
    bool directional;

    Light(bool d, Vec3 p, Vec3 a, Vec3 c) : position(p), attenuation(a), colour(c), directional(d ? 1 : 0) {}
};

#endif

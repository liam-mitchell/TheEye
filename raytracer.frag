#version 430

const float FLOAT_MAX = 0x7f7fffff;
const float EPSILON = 0.001f;

// Comments on these structs indicate offsets of the members when using the std430 layout.
// This is deeply important because we send the shader storage buffers all at once, rather
// than member by member - so the layout of the `std::vector<Light> lights` in C++ code
// must match the layout of the shader storage buffer `Light [] lights` in GLSL.
// In order to do this, we need to manually pad the C++ structures with the commented
// number of bytes - C++ structures default to being much more tightly packed than GLSL
// structures.
struct Light {
    vec3 position; // 12
    // pad 4
    vec3 attenuation; // 12
    // pad 4
    vec3 colour; // 12
    bool directional; // 1
    // pad 3
}; // 48

struct Material {
    vec3 colour; // 12
    // pad 4
    vec3 diffuse; // 12
    // pad 4
    vec3 specular; // 12
    // pad 4
    vec3 emission; // 12
    float shininess; // 4
}; // 64

struct Sphere {
    Material material; // 64
    vec3 position; // 12
    float radius; // 4
}; // 80

struct Triangle {
    Material material; // 64
    vec3 vertices[3]; // 48
    vec3 normal; // 12
}; // 124

struct Plane {
    Material material; // 64
    vec3 normal; // 12
    // pad 4
    vec3 point; // 12
    // pad 4
}; // 96

// This struct is set member-by-member using glUniform* - so the layout isn't important
// here. Each member must instead be set whenever it changes on the camera in C++ code.
struct Camera {
    vec3 position;
    vec3 u;
    vec3 v;
    vec3 w;
    float fov;

    float aperture;
    float focallength;
    bool dof;
    uint ndepthrays;
};

// These structs are just internal raytracer structs - nothing special about them.
struct Ray {
    vec3 origin;
    vec3 direction;
    float tmin;
    float tmax;
};

struct Intersection {
    vec3 point;
    vec3 normal;
    Material material;
};

// Shader storage buffer objects.
// Note that these use the std430 layout in order to make copying simpler from C++. This
// layout is documented in the OpenGL spec and guaranteed across platforms.
//
// The upshot of this is that we can make our C++ struct's layout exactly the same as that
// of the GLSL struct, and with that we can simply bind an std::vector's data to the
// buffer, instead of setting each element's members individually.
layout(binding = 0, std430) buffer Lights {
    Light [] lights;
};

layout(binding = 1, std430) buffer Spheres {
    Sphere [] spheres;
};

layout(binding = 2, std430) buffer Triangles {
    Triangle [] triangles;
};

layout(binding = 3, std430) buffer Planes {
    Plane [] planes;
};

uniform Camera camera;
uniform uint maxdepth;

in vec2 vPosition;
out vec4 colour;

// A reasonable pseudo-random number generator (good enough for our purposes).
// Seeded using a vec2 - from personal testing, it seems like its distribution works
// well when seeded with values scaling between 0 and 1.
//
// The oldest mention I can find of this on the internet is at
// web.archive.org/web/20080211204527/http://lumina.sourceforge.net/Tutorials/Noise.html
//
// More discussion can be found on stackoverflow:
// stackoverflow.com/questions/12964279/whats-the-origin-of-this-glsl-rand-one-liner
highp float rand(vec2 seed) {return fract(sin(dot(seed, vec2(12.9898, 78.233))) * 43758.5453);}

// Ray generation functions.
//
// These functions use the input vPosition (passed through from the vertex shader) to
// determine the point on the view plane to aim rays at. vPosition is normalized to
// the range ((-1, -1), (1, 1)), ie. standard OpenGL window coordinates.

// Generate a standard ray direct from the camera position through the vertex.
// Uses the camera's field of view to appropriately scale the x and y components of
// the generated ray's direction.
Ray generate_ray() {
    float scale = tan(radians(camera.fov) / 2);
    float x = scale * vPosition.x;
    float y = -scale * vPosition.y;

    vec3 dir = normalize(camera.u * x + camera.v * y + camera.w);
    return Ray(camera.position, dir, 0.0, FLOAT_MAX);
}

// Generate a depth-of-field ray starting from a random position in the camera aperture
// and pointing towards the vertex on the focal plane.
//
// The vertex on the focal plane is found using the vPosition inputs scaled by the camera's
// field of view and focal length.
//
// param: i - index of ray. Used to seed each ray's random position jitter
//            with a different value.
Ray generate_depth_ray(in uint i) {
    vec3 center = camera.position + camera.w * camera.focallength;
    float scale = tan(radians(camera.fov) / 2);

    float x = scale * vPosition.x * camera.focallength;
    float y = -scale * vPosition.y * camera.focallength;

    vec3 lookat = center + camera.u * x + camera.v * y;

    vec2 offset = {i, i};
    float dx = (-0.5f + rand(vPosition + offset)) * camera.aperture;
    float dy = (-0.5f + rand(vPosition - offset)) * camera.aperture;

    vec3 origin = camera.position + camera.u * dx + camera.v * dy;
    vec3 dir = normalize(lookat - origin);

    return Ray(origin, dir, 0.0, FLOAT_MAX);
}

// Intersection tests.
// Each type of primitive implements its own intersection test. However, they all use the
// same interface:
//
// param: primitive (plane, sphere, triangle) - the primitive to test for intersection
// param: ray - the ray to intersect with the primitive
//
// return: t - the intersection distance
// return: intersection - the intersection data
// return: bool - did the ray intersect the primitive?

bool intersect_plane(in Plane plane, in Ray ray, out float t, out Intersection intersect) {
    if (dot(ray.direction, plane.normal) == 0) return false;

    // Find the distance along the ray until we hit the plane - since we're not parallel
    // to it, we hit it eventually
    float dist = (dot(plane.point, plane.normal) - dot(ray.origin, plane.normal)) / dot(ray.direction, plane.normal);
    if (dist < ray.tmin || dist > ray.tmax) return false;

    t = dist;

    intersect.point = ray.origin + ray.direction * t;
    intersect.normal = plane.normal;
    intersect.material = plane.material;

    return true;
}

bool intersect_triangle(in Triangle tri, in Ray ray, out float t, out Intersection intersect) {
    if (dot(ray.direction, tri.normal) == 0) return false;

    // Intersect with the plane that the triangle lies on
    float dist = (dot(tri.vertices[0], tri.normal) - dot(ray.origin, tri.normal)) / dot(ray.direction, tri.normal);
    if (dist < ray.tmin || dist > ray.tmax) return false;

    vec3 hit = ray.origin + ray.direction * dist;

    // The inside-out test.
    //
    // Take each edge of the triangle. The point must be on the right-hand side of
    // all of these edges in order to be inside the triangle (ie. to have an intersection).
    //
    // Test this by first finding a vector from the start of the edge to the point.
    // Take the cross product of this and the edge. If the point is to the right of the
    // edge, by the right-hand rule the cross product will point out of the triangle
    // in the direction of its normal. If the point is to the left, the cross product
    // will point opposite the normal.
    //
    // Test the direction by dotting with the normal. If the answer is positive,
    // the vectors are in the same direction.
    vec3 e0 = tri.vertices[1] - tri.vertices[0];
    vec3 v0_hit = hit - tri.vertices[0];
    float u = dot(tri.normal, cross(e0, v0_hit));
    if (u < 0) return false;

    vec3 e1 = tri.vertices[2] - tri.vertices[1];
    vec3 v1_hit = hit - tri.vertices[1];
    float v = dot(tri.normal, cross(e1, v1_hit));
    if (v < 0) return false;

    vec3 e2 = tri.vertices[0] - tri.vertices[2];
    vec3 v2_hit = hit - tri.vertices[2];
    float w = dot(tri.normal, cross(e2, v2_hit));
    if (w < 0) return false;

    t = dist;
    intersect.point = hit;
    intersect.normal = tri.normal;
    intersect.material = tri.material;

    return true;
}

bool intersect_sphere(in Sphere sphere, in Ray ray, out float t, out Intersection intersect) {
    // TODO figure out how this shit even works again yolo
    float a = dot(ray.direction, ray.direction);
    float b = 2 * dot(ray.direction, ray.origin - sphere.position);
    float c = dot(ray.origin - sphere.position, ray.origin - sphere.position) - pow(sphere.radius, 2);

    float discriminant = b * b - 4 * a * c;
    if (discriminant < 0) return false;

    float r1 = (-b + sqrt(discriminant)) / (2 * a);
    float r2 = (-b - sqrt(discriminant)) / (2 * a);

    if (r1 < ray.tmin || r1 > ray.tmax) {
	if (r2 < ray.tmin || r2 > ray.tmax) return false;
	else t = r2;
    } else if (r2 < ray.tmin || r2 > ray.tmax) {
	t = r1;
    } else {
	t = min(r1, r2);
    }

    intersect.point = ray.origin + ray.direction * t;
    intersect.normal = normalize(intersect.point - sphere.position);
    intersect.material = sphere.material;

    return true;
}

// Wrapper for all the intersection functions. Loops over primitives and intersects with
// them one at a time.
//
// param: ray - ray to intersect all primitives with
//
// return: t - distance of intersection
// return: intersect - intersection data
// return: bool - did the ray intersect anything?
bool intersect_ray(in Ray ray, out float t, out Intersection intersect) {
    t = FLOAT_MAX;
    bool ret = false;
    uint l = triangles.length();

    for (uint i = 0; i < l; ++i) {
        float current_t = FLOAT_MAX;
        Intersection current_intersect;

        if (intersect_triangle(triangles[i], ray, current_t, current_intersect) && current_t < t) {
            t = current_t;
            intersect = current_intersect;
            ret = true;
        }
    }

    l = spheres.length();

    for (uint i = 0; i < l; ++i) {
	float current_t = FLOAT_MAX;
	Intersection current_intersect;

	if (intersect_sphere(spheres[i], ray, current_t, current_intersect) && current_t < t) {
	    t = current_t;
	    intersect = current_intersect;
	    ret = true;
	}
    }

    l = planes.length();

    for (uint i = 0; i < l; ++i) {
	float current_t = FLOAT_MAX;
	Intersection current_intersect;

	if (intersect_plane(planes[i], ray, current_t, current_intersect) && current_t < t) {
	    t = current_t;
	    intersect = current_intersect;
	    ret = true;
	}
    }

    return ret;
}

// Shadow ray intersections.
// Detects whether or not a light's rays are shaded by any object in the scene.
//
// param: light - light to check visibility of
// param: point - point to check visibility at
//
// return: bool - did the light ray intersect anything?
//                note that this is the opposite of the light's visibility at the point.

// Intersect with a directional light.
// Directional lights are represented just by a directional vector - if anything
// shades the point from that direction, no matter how far away, its light is considered
// shaded.
bool intersect_directional_shadow_ray(in Light light, in vec3 point) {
    vec3 dir = normalize(light.position);
    Ray ray = Ray(point, dir, EPSILON, FLOAT_MAX);

    float t = FLOAT_MAX;
    Intersection i;

    return intersect_ray(ray, t, i);
}

// Intersect with a point light.
// Point lights are represented by a position vector - they radiate evenly from all sides.
// If the ray between the light and the point intersects anything, the light is shaded.
bool intersect_point_shadow_ray(in Light light, in vec3 point) {
    vec3 vec = light.position - point;

    float tmax = length(vec);
    vec3 dir = normalize(vec);

    Ray ray = Ray(point, dir, EPSILON, tmax);

    float t = FLOAT_MAX;
    Intersection i;

    return intersect_ray(ray, t, i);
}

// Intersect whatever type of shadow ray is required for this light.
bool intersect_shadow_ray(in Light light, in vec3 point) {
    if (light.directional) return intersect_directional_shadow_ray(light, point);
    else return intersect_point_shadow_ray(light, point);
}

// Compute the pixel colour of a ray intersection.
// Note that this doesn't handle tracing reflection rays - since GLSL can't do recursion,
// we have to do those iteratively in the trace_ray() function.
vec3 compute_colour(in Intersection intersect, in Ray ray) {
    vec3 dir = normalize(ray.origin - intersect.point);
    vec3 colour = intersect.material.colour * intersect.material.diffuse;

    uint l = lights.length();

    // Loop over all lights, adding each one's specular and diffuse contributions
    for (uint i = 0; i < l; ++i) {
        // If this light is shaded, don't add anything
        if (intersect_shadow_ray(lights[i], intersect.point)) continue;

        // Lambert model of diffuse shading
        vec3 lightvec = normalize(lights[i].directional ? lights[i].position : (lights[i].position - intersect.point));
        float nDotL = dot(intersect.normal, lightvec);
        vec3 lambert = intersect.material.diffuse * lights[i].colour * max(nDotL, 0.0);

        // Phong model of specular shading
        vec3 halfvec = normalize(dir + lightvec);
        float nDotH = dot(intersect.normal, halfvec);
        vec3 phong = intersect.material.specular * lights[i].colour * pow(max(nDotH, 0), intersect.material.shininess);

        // Attenuation factor of the light - how strong is it this far away?
        // For directional lights this is always just the strength of the light - for
        // point lights it falls off with distance.
        float d = length(lights[i].position - intersect.point);
        float attenuation = 1.0f / (lights[i].attenuation.x + lights[i].attenuation.y * d + lights[i].attenuation.z * pow(d, 2));

        // Add the colour scaled by the attenuation.
        colour += (lambert + phong) * attenuation;
    }

    return colour;
}

// Trace a ray and its reflections, adding the computed colour for each intersection
// along the way.
//
// This function would maybe be more natural expressed as recursion, but GLSL doesn't
// do recursion so we have to do it in a loop instead, multiplying each successive
// reflection ray by the specular factor of the objects it's reflected off of.
vec4 trace_ray(in Ray ray) {
    uint depth = 0;
    vec3 ret = {0, 0, 0};

    // Specular factor to multiply by
    // Starts at 1.0 on all components because the first ray contributes 100% of its
    // value, then reflection rays get progressively lower contributions based on
    // specular reflection
    vec3 factor = {1.0, 1.0, 1.0};

    while (depth < maxdepth) {
        Intersection intersect;
        float t;

        if (intersect_ray(ray, t, intersect)) {
            // Add the colour of this ray
            ret += compute_colour(intersect, ray) * factor;

            // Scale the reflection ray's contribution by the specular factor
            factor *= intersect.material.specular;

            // Trace a reflection ray next loop
            vec3 indir = -ray.direction;
            vec3 dir = normalize(intersect.normal * dot(indir, intersect.normal) * 2 - indir);

            ray = Ray(intersect.point, dir, EPSILON, FLOAT_MAX);
        } else {
            return vec4(ret, 1.0);
        }

        ++depth;
    }

    return vec4(ret, 1.0);
}

// Trace depth rays
// Wrapper around generate_depth_ray()/trace_ray() in a loop up to camera.ndepthrays.
vec4 trace_rays() {
    vec4 ret = {0, 0, 0, 1};

    for (uint i = 0; i < camera.ndepthrays; ++i) {
        Ray ray = generate_depth_ray(i);
        ret += trace_ray(ray);
    }

    ret /= camera.ndepthrays;
    return ret;
}

void main() {
    if (camera.dof) {
        colour = trace_rays();
    } else {
        Ray ray = generate_ray();
        colour = trace_ray(ray);
    }
}

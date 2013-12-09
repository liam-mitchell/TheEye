#ifndef PARSER__H_
#define PARSER__H_

#include <sstream>
#include <stack>
#include "Mat4.h"
#include "Scene.h"

// class Parser
// responsible for parsing scene description language from scene file
// and building a scene based on commands therein
// friend of Scene class to allow it to construct scenes

typedef class Parser
{
public:
	Parser();
	void buildSceneFromFile(Scene &scene, const std::string &filename);

private:
	bool readvals(std::stringstream &s, float *dest, const int numvals);
	void rightmult(const Mat4 &m, std::stack<Mat4> &t);

private:
	float params[10]; //file read variables
	
	std::stack<Mat4> transforms; //stack to store heirarchical transforms

	Point *verts; //array to be allocated with maxverts command, indexed by vertindex
	unsigned int vertindex;

	Mat4 global_inverse_translation; //inverse of global camera translation
	Mat4 translation; //transformation matrices for primitives
	Mat4 rotation;
	Mat4 scale;
	Mat4 inverse_translation;
	Mat4 inverse_rotation;
	Mat4 inverse_scale;

	Vec3 attenuation; //attenuation for lights

	Vec3 ambient; //material properties for primitives
	Vec3 diffuse;
	Vec3 specular;
	Vec3 emission;
	float shininess;
} Parser;

#endif
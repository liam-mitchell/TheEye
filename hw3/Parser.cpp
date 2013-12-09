#include "Parser.h"
#include <iostream>
#include <fstream>
#include "DirectionalLight.h"
#include "PointLight.h"
#include "Ellipse.h"
#include "Triangle.h"

Parser::Parser()
{
	shininess = 0;

	global_inverse_translation = translation = rotation = scale = inverse_translation = inverse_scale = inverse_rotation = Mat4(1.0);

	attenuation = Vec3(1.0f, 0.0f, 0.0f);

	verts = nullptr;
	vertindex = 0;
}

// buildSceneFromFile()
// params: empty scene to be created, scene filename (using the CS184 scene description language)
// return: none
// notes: parses the scene description language into a fully fleshed scene (as long as it's fully described in the scene file!)

void Parser::buildSceneFromFile(Scene &scene, const std::string &filename)
{
	std::string str, cmd; //strings for current line of file and current command
	std::ifstream infile;

	infile.open(filename);
	if (infile.is_open())
	{
		getline(infile, str);

		while(infile)
		{
			if ( (str.find_first_not_of(" \t\r\n") != std::string::npos) && (str[0] != '#')) //rule out comments & blank lines
			{
				std::stringstream ss(str); //create stringstream to read values one at a time, command first
				ss >> cmd;

				bool valid; //check validity of input before we do anything with it
			
				if (cmd == "size") // set scene width and height
				{
					valid = readvals(ss, params, 2);
					if (valid)
					{
						scene.m_width = static_cast<int>(params[0]);
						scene.m_height = static_cast<int>(params[1]);
					}
				}

				if (cmd == "maxdepth") //waste the values for maxdepth/output - we don't want to parse them
				{
					int temp;
					ss >> temp;
				}

				if (cmd == "output")
				{
					ss >> str;
				}

				if (cmd == "camera") //camera creation
				{
					valid = readvals(ss, params, 10);
					if (valid)
					{
						Camera cam(params[0], params[1], params[2], params[3], params[4], params[5], params[6], params[7], params[8], params[9]);
						scene.m_camera = cam;
					}
				}

				if (cmd == "maxverts") //vertex specifications
				{
					valid = readvals(ss, params, 1);
					if (valid)
					{
						verts = new Point[static_cast<int>(params[0])]; //allocate space for all the vertices promised in the scene file - must come before verts are specified! Segfaults galore if not
					}
				}

				if (cmd == "vertex")
				{
					valid = readvals(ss, params, 3);
					if (valid)
					{
						Point p(params[0], params[1], params[2]);
						verts[vertindex] = p; //add to the vertex array, then increment the index counter (so we add the next one later...)
						++vertindex;
					}
				}

				if (cmd == "attenuation") //lighting and material settings
				{
					valid = readvals(ss, params, 3);
					if (valid)
					{
						Vec3 a(params[0], params[1], params[2]);
						attenuation = a;
					}
				}

				if (cmd == "ambient")
				{
					valid = readvals(ss, params, 3);
					if (valid)
					{
						Vec3 a(params[0], params[1], params[2]);
						ambient = a;
					}
				}

				if (cmd == "diffuse")
				{
					valid = readvals(ss, params, 3);
					if (valid)
					{
						Vec3 d(params[0], params[1], params[2]);
						diffuse = d;
					}
				}

				if (cmd == "specular")
				{
					valid = readvals(ss, params, 3);
					if (valid)
					{
						Vec3 s(params[0], params[1], params[2]);
						specular = s;
					}
				}

				if (cmd == "emission")
				{
					valid = readvals(ss, params, 3);
					if (valid)
					{
						Vec3 e(params[0], params[1], params[2]);
						emission = e;
					}
				}

				if (cmd == "shininess")
				{
					valid = readvals(ss, params, 1);
					if (valid)
					{
						shininess = params[0];
					}
				}

				if (cmd == "directional") //light specifications
				{
					valid = readvals(ss, params, 6);
					if (valid)
					{
						Vec3 d(params[0], params[1], params[2]);
						Vec3 c(params[3], params[4], params[5]);
						std::unique_ptr<Light> l(new DirectionalLight(d, c));
						scene.addLight(std::move(l));
					}
				}

				if (cmd == "point")
				{
					valid = readvals(ss, params, 6);
					if (valid)
					{
						Vec3 o(params[0], params[1], params[2]);
						Vec3 c(params[3], params[4], params[5]);
						std::unique_ptr<Light> l(new PointLight(o, c, attenuation));
						scene.addLight(std::move(l));
					}
				}

				if (cmd == "pushTransform") //transform stack management commands
				{
					if (transforms.empty()) transforms.push(Mat4(1.0));
					else (transforms.push(transforms.top()));
				}

				if (cmd == "popTransform")
				{
					if (transforms.empty()) std::cerr << "Trying to pop empty stack!\n";
					else
					{
						transforms.pop();

						if (!transforms.empty())
						{
							translation = transforms.top();
							inverse_translation = global_inverse_translation; //some awkwardness to maintain inverse translation and translation matrices properly, even when there are global transforms
																			  //wouldn't be necessary if our mat4 could invert itself... T.T
						}
						else
						{
							translation = inverse_translation = Mat4(1.0);
						}
						rotation = inverse_rotation = scale = inverse_scale = Mat4(1.0);
					}
				}

				if (cmd == "translate") //transformations to be applied to the top of the stack
				{
					valid = readvals(ss, params, 3);
					if (valid)
					{
						Mat4 current_trans = Mat4::translation(params[0], params[1], params[2]); //Track inverse of the transforms we've applied, for creation of ellipses - since our mat4 class doesn't have inversion capability, we have to do it by hand
						Mat4 current_inv_trans = Mat4::translation(-params[0], -params[1], -params[2]); 

						rightmult(current_trans, transforms);

						translation *= current_trans; //combine the current translation with the overall translation
						inverse_translation *= current_inv_trans;

						if (transforms.size() == 1)
						{
							global_inverse_translation *= current_inv_trans; //if this is a global camera translation and not a specific object, take note!
						}
					}
				}

				if (cmd == "rotate")
				{
					valid = readvals(ss, params, 4);
					if (valid)
					{
						Mat4 current_rot = Mat4::rotation(params[3], Vec3(params[0], params[1], params[2]));
						Mat4 current_inv_rot = Mat4::rotation(-params[3], Vec3(params[0], params[1], params[2]));
						rightmult(current_rot, transforms);

						rotation *= current_rot;
						inverse_rotation *= current_inv_rot;
					}
				}

				if (cmd == "scale")
				{
					valid = readvals(ss, params, 3);
					if (valid)
					{
						Mat4 current_scale = Mat4::scale(params[0], params[1], params[2]);
						Mat4 current_inv_scale = Mat4::scale(1.0f / params[0], 1.0f / params[1], 1.0f / params[2]);
						rightmult(current_scale, transforms);

						scale *= current_scale;
						inverse_scale *= current_inv_scale;
					}
				}

				if (cmd == "sphere") //object definitions
				{
					valid = readvals(ss, params, 4);
					if (valid)
					{
						Mat4 current_scale = Mat4::scale(params[3], params[3], params[3]);
						Mat4 current_inv_scale = Mat4::scale(1.0f / params[3], 1.0f / params[3], 1.0f / params[3]);

						Mat4 current_trans = Mat4::translation(params[0], params[1], params[2]);
						Mat4 current_inv_trans = Mat4::translation(-params[0], -params[1], -params[2]);

						current_scale *= scale;
						current_inv_scale *= inverse_scale;

						current_trans *= translation;
						current_inv_trans *= inverse_translation;

						std::unique_ptr<Primitive> e(new Ellipse(rotation, current_trans, current_scale, inverse_rotation, current_inv_trans, current_inv_scale, ambient, diffuse, specular, emission, shininess));
						scene.addObject(std::move(e)); //all spheres are currently represented as ellipses - could be more efficient with a little more parsing
					}
				}

				if (cmd == "tri")
				{
					valid = readvals(ss, params, 3);
					if (valid)
					{
						int i0 = static_cast<int>(params[0]);
						int i1 = static_cast<int>(params[1]);
						int i2 = static_cast<int>(params[2]);

						Vec3 vert1 = verts[i0];
						Vec3 vert2 = verts[i1];
						Vec3 vert3 = verts[i2];

						if (!transforms.empty()) //transform vertices by the current model transform
						{
							vert1 = transforms.top() * vert1;
							vert2 = transforms.top() * vert2;
							vert3 = transforms.top() * vert3;
						} //otherwise don't transform, there's no transform to apply

						std::unique_ptr<Primitive> t(new Triangle(vert1, vert2, vert3, ambient, diffuse, specular, emission, shininess));
						scene.addObject(std::move(t));
					}
				}
			}

			getline(infile, str); //read the next command
		}
	}

	else
	{
		std::cerr << "Unable to open file!\n";
	}
}

// readvals()
// params: string to read from, float array to read into, number of values to read
// return: bool succesful read
// notes:

bool Parser::readvals(std::stringstream &s, float *dest, const int numvals)
{
	for (int i = 0; i < numvals; i++) {
        s >> params[i]; 
        if (s.fail()) {
            std::cout << "Failed reading value " << i << " will skip\n"; 
            return false;
        }
    }
    return true; 
}

// rightmult()
// params: matrix to multiply, stack to right-multiply onto
// return: void
// notes: right-multiplies the top of the stack by m

void Parser::rightmult(const Mat4 &m, std::stack<Mat4> &t)
{
	Mat4 &top = t.top();
	top *= m;
}
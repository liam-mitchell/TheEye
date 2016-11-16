#ifndef SCENE__H_
#define SCENE__H_

#include <string>
#include <memory>
#include <vector>

#include "Primitive.h"
#include "Light.h"
#include "Camera.h"
#include "Window.h"

// class Scene
// stores all objects and lights in the scene, as well as the camera
// and renders them from the camera's pov when asked
// can be built from Parser::buildSceneFromFile - thus Parser is a friend

class Scene 
{
public:
	Scene();
	Scene(const Camera &cam, const int width, const int height);
	
	void addSphere(Sphere sphere);
	void addTriangle(Triangle triangle);
	void addPlane(Plane plane);
	void addLight(Light light);

        void update(float dt);
	void render();
	void save();
	int width() {return m_width;}
	int height() {return m_height;}

	Camera &camera() {return m_camera;}

	friend class Parser;
private:
        void createGeometry();

	std::vector<Light> m_lights;
	std::vector<Sphere> m_spheres;
        std::vector<Plane> m_planes;
	std::vector<Triangle> m_triangles;

        Window m_window;
	Camera m_camera;
	int m_width, m_height;
	Shader m_shader;
        GLuint m_vertexArray;
        bool m_dirty;
};

#endif

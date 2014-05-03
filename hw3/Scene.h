#ifndef SCENE__H_
#define SCENE__H_

#include <string>
#include <list>
#include <memory>
#include "Primitive.h"
#include "Light.h"
#include "Image.h"
#include "Camera.h"

// class Scene
// stores all objects and lights in the scene, as well as the camera
// and renders them from the camera's pov when asked
// can be built from Parser::buildSceneFromFile - thus Parser is a friend

class Primitive;
class Light;

typedef class Scene 
{
public:
	Scene() {}
	Scene(const Camera &cam, const int width, const int height);
	
	void addObject(std::unique_ptr<Primitive> obj);
	void addLight(std::unique_ptr<Light> light);
	Image render();
	void partialRender(Image &image, int x, int y, int width, int height);
	Image generateBlankImage();
	int width() {return m_width;}
	int height() {return m_height;}

	Camera &camera() {return m_camera;}

	friend class Parser;
private: //disable assignment and copying
	Scene(const Scene &rhs);
	void operator=(const Scene &rhs);
private:
	std::list<std::unique_ptr<Primitive> > m_primitives;
	std::list<std::unique_ptr<Light> > m_lights;
	Camera m_camera;
	int m_width, m_height;
} Scene;

#endif
#include "Scene.h"
#include "Raytracer.h"
#include "Primitive.h"
#include "Light.h"
#include <iostream>

Scene::Scene(const Camera &cam, const int width, const int height)
{
	m_camera = cam;
	m_width = width;
	m_height = height;
}

// addObject()
// params: object to add to scene
// return: none
// notes: moves the pointer back onto the list

void Scene::addObject(std::unique_ptr<Primitive> obj)
{
	m_primitives.push_back(std::move(obj));
}

// addLight()
// params: light to add
// return: none
// notes: moves the pointer back onto the list

void Scene::addLight(std::unique_ptr<Light> light)
{
	m_lights.push_back(std::move(light));
}

// render()
// params: none
// return: image of the scene as seen from the scene's camera
// notes:

Image Scene::render()
{
	Image image(m_width, m_height);
	Raytracer rt;

	int pixels = m_width * m_height;

	for (int j = 0; j < m_height; ++j)
	{
		if (j % 10 == 0)
		{
			std::cout << "Pixel " << j * m_width << " of " << pixels << " traced...\n";
		}
		for (int i = 0; i < m_width; ++i)
		{
			Sample s = Sampler::sample(m_width, m_height, i + .5f, j + .5f);
			Ray r = m_camera.generateRay(s, (float)m_width / m_height);
			Color c = rt.trace(r, m_primitives, m_lights);
			image.setPixel(i, j, c);
		}
	}

	return image;
}
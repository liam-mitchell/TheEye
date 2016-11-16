#include "Scene.h"
#include "Primitive.h"
#include "Light.h"
#include "ImageWriter.hpp"

#define GLFW_INCLUDE_GLCOREARB
#define GL_GLEXT_PROTOTYPES
#include <GLFW/glfw3.h>

#include <iostream>
#include <thread>
#include <map>

enum KeyState {UP, DOWN, HOLD};

static std::map<int, KeyState> input;
static const float CAMVELOCITY = 1.0f;

static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
        if (action == GLFW_PRESS) {
                input[key] = KeyState::DOWN;
        } else if (action == GLFW_RELEASE) {
                input[key] = KeyState::UP;
        }
}

void Scene::createGeometry() {
	GLfloat vertices[][2] = {
		{-1.0f, -1.0f},
		{-1.0f, 1.0f},
		{1.0f, 1.0f},
		{1.0f, -1.0f}
	};

	GLuint vertexBuffer = 0;

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &m_vertexArray);
	glBindVertexArray(m_vertexArray);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
}

Scene::Scene(const Camera &cam, const int width, const int height)
	: m_camera(cam),
	  m_width(width),
	  m_height(height),
          m_shader(Shader("passthrough.vert", "raytracer.frag"))
{
	createGeometry();
        glfwSetKeyCallback(m_window.window(), keyCallback);
}

Scene::Scene()
        : m_shader(Shader("passthrough.vert", "raytracer.frag"))
{
	createGeometry();
        glfwSetKeyCallback(m_window.window(), keyCallback);
}

void Scene::addSphere(Sphere sphere)
{
	m_spheres.push_back(sphere);
        m_dirty = true;
}

void Scene::addTriangle(Triangle tri)
{
	m_triangles.push_back(tri);
        m_dirty = true;
}

void Scene::addPlane(Plane plane)
{
	m_planes.push_back(plane);
        m_dirty = true;
}

void Scene::addLight(Light light)
{
	m_lights.push_back(light);
        m_dirty = true;
}

static void updateCameraTransform(Camera &cam, float dt) {
        if (input[GLFW_KEY_UP] == HOLD) {
                cam.pos += cam.w * CAMVELOCITY * dt;
        } else if (input[GLFW_KEY_DOWN] == HOLD) {
                cam.pos -= cam.w * CAMVELOCITY * dt;
        } else if (input[GLFW_KEY_RIGHT] == HOLD) {
                cam.pos += cam.u * CAMVELOCITY * dt;
        } else if (input[GLFW_KEY_LEFT] == HOLD) {
                cam.pos -= cam.u * CAMVELOCITY * dt;
        }
}

static void updateCameraDepth(Camera &cam, float dt) {
	if (input[GLFW_KEY_UP] == HOLD) {
		cam.focallength += CAMVELOCITY * dt;
	} else if (input[GLFW_KEY_DOWN] == HOLD) {
		cam.focallength -= CAMVELOCITY * dt;
	} else if (input[GLFW_KEY_RIGHT] == HOLD) {
		cam.aperture += CAMVELOCITY / 10 * dt;
	} else if (input[GLFW_KEY_LEFT] == HOLD) {
		cam.aperture -= CAMVELOCITY / 10 * dt;
	}
}

void Scene::save()
{
	static unsigned int nfiles = 0;
	std::string filename("screenshot");
	filename += std::to_string(nfiles++) + ".jpg";

	m_camera.ndepthrays = 64;
	render();

	ImageWriter writer(512, 512);
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glReadPixels(0, 0, 512, 512, GL_RGBA, GL_UNSIGNED_BYTE, writer.get_data());
	writer.save_opengl(filename.c_str());

	m_camera.ndepthrays = 4;
}

void Scene::update(float dt)
{
	if (input[GLFW_KEY_2] == DOWN) {
		m_camera.dof = true;
	} else if (input[GLFW_KEY_1] == DOWN) {
		m_camera.dof = false;
	} else if (input[GLFW_KEY_3] == DOWN) {
		save();
	}

	if (m_camera.dof) {
		updateCameraDepth(m_camera, dt);
	} else {
		updateCameraTransform(m_camera, dt);
	}

        for (auto &i: input) {
                if (i.second == KeyState::DOWN) i.second = KeyState::HOLD;
        }
}

void Scene::render()
{
	glUseProgram(m_shader.program());

        if (m_dirty) {
                GLuint buffers[4] = {0, 0, 0};
                GLuint lengths[4] = {
                        static_cast<GLuint>(sizeof(Light) * m_lights.size()),
                        static_cast<GLuint>(sizeof(Sphere) * m_spheres.size()),
                        static_cast<GLuint>(sizeof(Triangle) * m_triangles.size()),
                        static_cast<GLuint>(sizeof(Plane) * m_planes.size())
                };
                
                glGenBuffers(4, buffers);
                
                glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, buffers[0]);
                glBufferData(GL_SHADER_STORAGE_BUFFER, lengths[0], m_lights.data(), GL_DYNAMIC_DRAW);
                
                glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, buffers[1]);
                glBufferData(GL_SHADER_STORAGE_BUFFER, lengths[1], m_spheres.data(), GL_DYNAMIC_DRAW);
                
                glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, buffers[2]);
                glBufferData(GL_SHADER_STORAGE_BUFFER, lengths[2], m_triangles.data(), GL_DYNAMIC_DRAW);
                
                glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, buffers[3]);
                glBufferData(GL_SHADER_STORAGE_BUFFER, lengths[3], m_planes.data(), GL_DYNAMIC_DRAW);
        }

        glUniform3f(glGetUniformLocation(m_shader.program(), "camera.position"), m_camera.pos.x, m_camera.pos.y, m_camera.pos.z);
        glUniform3f(glGetUniformLocation(m_shader.program(), "camera.u"), m_camera.u.x, m_camera.u.y, m_camera.u.z);
        glUniform3f(glGetUniformLocation(m_shader.program(), "camera.v"), m_camera.v.x, m_camera.v.y, m_camera.v.z);
        glUniform3f(glGetUniformLocation(m_shader.program(), "camera.w"), m_camera.w.x, m_camera.w.y, m_camera.w.z);
        glUniform1f(glGetUniformLocation(m_shader.program(), "camera.fov"), m_camera.fovy);
        glUniform1f(glGetUniformLocation(m_shader.program(), "camera.aperture"), m_camera.aperture);
        glUniform1f(glGetUniformLocation(m_shader.program(), "camera.focallength"), m_camera.focallength);
        glUniform1i(glGetUniformLocation(m_shader.program(), "camera.dof"), m_camera.dof);
        glUniform1ui(glGetUniformLocation(m_shader.program(), "camera.ndepthrays"), m_camera.ndepthrays);
        glUniform1ui(glGetUniformLocation(m_shader.program(), "maxdepth"), 5);

	glBindVertexArray(m_vertexArray);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

        glfwSwapBuffers(m_window.window());
}

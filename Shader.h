#ifndef __SHADER_H_
#define __SHADER_H_

#define GLFW_INCLUDE_GLCOREARB
#define GL_GLEXT_PROTOTYPES
#include <GLFW/glfw3.h>

#include <string>

class Shader {
public:
    Shader();
    Shader(const char *vertex, const char *fragment, const char *tesscontrol = NULL, const char *tesseval = NULL);
    ~Shader();
    Shader(const Shader &other);

    GLuint program() const {return program_;}

    // virtual void render(const Geometry &geometry) const;
protected:
    GLuint vertex_;
    GLuint fragment_;
    GLuint tesscontrol_;
    GLuint tesseval_;
    GLuint program_;
};

std::string LoadSource(const char *filename);
GLuint LinkProgram(GLuint vertex, GLuint fragment);
GLuint CompileShader(GLenum shaderType, const std::string &source);

#endif // __SHADER_H_

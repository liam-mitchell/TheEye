#include "util.h"

#define GLFW_INCLUDE_GLCOREARB
#define GL_GLEXT_PROTOTYPES
#include <GLFW/glfw3.h>

#include <sstream>

bool CheckGLErrors()
{
    bool error = false;
    std::ostringstream err;
    err << "OpenGL error: ";
    for (GLenum flag = glGetError(); flag != GL_NO_ERROR; flag = glGetError())
    {
        switch (flag) {
        case GL_INVALID_ENUM:
            err << "GL_INVALID_ENUM" << endl; break;
        case GL_INVALID_VALUE:
            err << "GL_INVALID_VALUE" << endl; break;
        case GL_INVALID_OPERATION:
            err << "GL_INVALID_OPERATION" << endl; break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            err << "GL_INVALID_FRAMEBUFFER_OPERATION" << endl; break;
        case GL_OUT_OF_MEMORY:
            err << "GL_OUT_OF_MEMORY" << endl; break;
        default:
            err << "[unknown error code]" << endl;
        }
        error = true;
    }

    if (error) {
        throw std::runtime_error(err.str());
    }

    return false;
}

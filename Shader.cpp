#include "Shader.h"

#include <fstream>
#include <sstream>

#include "util.h"

GLuint LinkProgram(GLuint vertex, GLuint fragment, GLuint tesscontrol, GLuint tesseval)
{
    // allocate program object name
    GLuint program = glCreateProgram();

    CheckGLErrors();

    // attach provided shader objects to this program
    if (vertex) {
        glAttachShader(program, vertex);
    }

    CheckGLErrors();

    if (fragment) {
        glAttachShader(program, fragment);
    }

    if (tesscontrol) {
        glAttachShader(program, tesscontrol);
    }

    if (tesseval) {
        glAttachShader(program, tesseval);
    }

    CheckGLErrors();

    // try linking the program with given attachments
    glLinkProgram(program);

    CheckGLErrors();

    // retrieve link status
    GLint status = GL_FALSE;
    glGetProgramiv(program, GL_LINK_STATUS, &status);

    if (status == GL_FALSE)
    {
        GLint length;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
        std::string info(length, ' ');
        glGetProgramInfoLog(program, info.length(), &length, &info[0]);

        std::ostringstream err;
        err << "error linking shading program: \n" << info;

        throw std::runtime_error(err.str());
    }

    return program;
}

std::string LoadSource(const char *filename) {
    std::ifstream input(filename);

    if (!input) {
        std::ostringstream err;
        err << "could not load shader source from file " << filename;

        throw std::runtime_error(err.str());
    }

    return std::string(std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>());
}

GLuint CompileShader(GLenum shaderType, const std::string &source)
{
    // allocate shader object name
    GLuint shader = glCreateShader(shaderType);

    // try compiling the source as a shader of the given type
    const GLchar *source_ptr = source.c_str();
    glShaderSource(shader, 1, &source_ptr, NULL);
    glCompileShader(shader);

    // retrieve compile status
    GLint status = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE)
    {
        GLint length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        std::string info(length, ' ');
        glGetShaderInfoLog(shader, info.length(), &length, &info[0]);

        std::ostringstream err;
        err << "compiling shader: \n" << source << "\n" << info << "\n";

        throw std::runtime_error(err.str());
    }

    return shader;
}

Shader::Shader() : Shader("hw3/vertex.glsl", "hw3/fragment.glsl") {}
Shader::Shader(const char *vertex, const char *fragment, const char *tesscontrol, const char *tesseval) :
  vertex_(0),
  fragment_(0),
  tesscontrol_(0),
  tesseval_(0),
  program_(0)
{
    std::string vertexSource(LoadSource(vertex));
    std::string fragmentSource(LoadSource(fragment));
    std::string tesscontrolSource;
    std::string tessevalSource;

    if (tesscontrol) {
        tesscontrolSource = LoadSource(tesscontrol);
    }

    if (tesseval) {
        tessevalSource = LoadSource(tesseval);
    }

    if (vertexSource.empty() || fragmentSource.empty()) {
        throw std::runtime_error("failed to load shader");
    }

    vertex_ = CompileShader(GL_VERTEX_SHADER, vertexSource);
    fragment_ = CompileShader(GL_FRAGMENT_SHADER, fragmentSource);

    CheckGLErrors();

    if (tesscontrol) {
        tesscontrol_ = CompileShader(GL_TESS_CONTROL_SHADER, tesscontrolSource);
    }

    if (tesseval) {
        tesseval_ = CompileShader(GL_TESS_EVALUATION_SHADER, tessevalSource);
    }

    program_ = LinkProgram(vertex_, fragment_, tesscontrol_, tesseval_);

    if (CheckGLErrors()) {
        throw std::runtime_error("GL error while compiling shader");
    }
}

Shader::~Shader() {
    glUseProgram(0);
    glDeleteProgram(program_);
    glDeleteShader(vertex_);
    glDeleteShader(fragment_);
}

Shader::Shader(const Shader &other)
: vertex_(other.vertex_),
  fragment_(other.fragment_),
  program_(other.program_)
{}

// void Shader::render(const Geometry &geometry) const {
    // glUseProgram(program_);

    // glActiveTexture(GL_TEXTURE0);
    // glBindTexture(GL_TEXTURE_RECTANGLE, image.texture());
    // glUniform1i(glGetUniformLocation(program_, "tex"), 0);

    // glUniformMatrix3fv(glGetUniformLocation(program_, "transform"),
    //                    1,
    //                    GL_FALSE,
    //                    (GLfloat *)image.transform().m_data);
    // glUniformMatrix3fv(glGetUniformLocation(program_, "kernel"),
    //                    1,
    //                    GL_FALSE,
    //                    (GLfloat *)kernel);

    // glUniform1fv(glGetUniformLocation(program_, "gaussian"),
    //              gaussianSize,
    //              (GLfloat *)gaussianKernel);

    // glUniform1ui(glGetUniformLocation(program_, "gaussianSize"), gaussianSize);

    // glPointSize(5.0f);

    // geometry.draw();

    // glBindVertexArray(geometry.vertexArray());
    // glDrawArrays(geometry.type(), 0, geometry.elementCount());

    // glBindVertexArray(0);
    // glUseProgram(0);
// }

#ifndef __WINDOW_H_
#define __WINDOW_H_

#include "Shader.h"

class Window {
public:
    Window();
    ~Window();

    GLFWwindow *window() {return window_;}
private:
    GLFWwindow *window_;
};

#endif // __WINDOW_H_

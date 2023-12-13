#pragma once

#include <GLFW/glfw3.h>

#include "pch.h"

namespace potatoengine {

class OpenGLContext {
  public:
    OpenGLContext(GLFWwindow* w);

    void init();
    void makeContextCurrent(GLFWwindow* w);
    void swapBuffers();

    static std::unique_ptr<OpenGLContext> Create(GLFWwindow* w);

  private:
    GLFWwindow* m_window{};
};
}
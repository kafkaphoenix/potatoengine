#pragma once

#include <GLFW/glfw3.h>

#include "potatoengine/pch.h"

namespace potatoengine {

class OpenGLContext {
   public:
    OpenGLContext(GLFWwindow* w);

    void init() const;
    void swapBuffers() const noexcept;

    static std::unique_ptr<OpenGLContext> Create(GLFWwindow* w);

   private:
    GLFWwindow* m_window{};
};
}
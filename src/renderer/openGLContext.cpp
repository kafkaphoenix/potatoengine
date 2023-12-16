#include "renderer/openGLContext.h"

#include <glad/glad.h>

namespace potatoengine {

OpenGLContext::OpenGLContext(GLFWwindow* w) : m_window(w) {
  ENGINE_ASSERT(w, "Window is null!");
}

void OpenGLContext::init() {
  makeContextCurrent(m_window);

  ENGINE_ASSERT(gladLoadGLLoader(
                  reinterpret_cast<GLADloadproc>(glfwGetProcAddress)) not_eq 0,
                "Failed to initialize Glad!");
}

void OpenGLContext::makeContextCurrent(GLFWwindow* w) {
  // TODO check with multi window if this works
  glfwMakeContextCurrent(w);
}

void OpenGLContext::swapBuffers() { glfwSwapBuffers(m_window); }

std::unique_ptr<OpenGLContext> OpenGLContext::Create(GLFWwindow* w) {
  return std::make_unique<OpenGLContext>(w);
}
}
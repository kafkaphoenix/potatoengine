#include "potatoengine/renderer/openGLContext.h"

#include <glad/glad.h>

namespace potatoengine {

OpenGLContext::OpenGLContext(GLFWwindow* w) : m_window(w) {
    if (not w) [[unlikely]] {
        throw std::runtime_error("Window is null!");
    }
}

void OpenGLContext::init() {
    makeContextCurrent(m_window);

    if (int status = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)); status == 0) {
        throw std::runtime_error("Failed to initialize Glad!");
    }
}

void OpenGLContext::makeContextCurrent(GLFWwindow* w) {
    // TODO check with multi window if this works
    glfwMakeContextCurrent(w);
}

void OpenGLContext::swapBuffers() {
    glfwSwapBuffers(m_window);
}

std::unique_ptr<OpenGLContext> OpenGLContext::Create(GLFWwindow* w) {
    return std::make_unique<OpenGLContext>(w);
}
}
#include "potatoengine/renderer/openGLContext.h"

#include <glad/glad.h>

namespace potatoengine {

OpenGLContext::OpenGLContext(GLFWwindow* window) : m_window(window) {
    if (window == nullptr) {
        fprintf(stderr, "Window is null!\n");
        throw std::runtime_error("Window is null!");
    }
}

void OpenGLContext::init() {
    glfwMakeContextCurrent(m_window);

    if (int status = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)); status == 0)
        fprintf(stderr, "Failed to initialize Glad!\n");
}

void OpenGLContext::swapBuffers() {
    glfwSwapBuffers(m_window);
}

std::unique_ptr<OpenGLContext> OpenGLContext::Create(void* window) {
    return std::make_unique<OpenGLContext>(static_cast<GLFWwindow*>(window));
}
}
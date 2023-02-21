#include "src/renderer/openGLContext.hpp"
#include "src/debug/GLDebugMessageCallback.h" // TODO: REMOVE

namespace potatocraft {
    OpenGLContext::OpenGLContext(GLFWwindow* window): m_window(window) {
        if (window == nullptr)
        {
            fprintf(stderr, "Window is null!\n");
        }
    }

    void OpenGLContext::init() {
        glfwMakeContextCurrent(m_window);

        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        if (status == 0)
        {
            fprintf(stderr, "Failed to initialize Glad!\n");
        }

        // glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(GLDebugMessageCallback, nullptr);
    }

    void OpenGLContext::swapBuffers() {
        glfwSwapBuffers(m_window);
    }

    Scope<OpenGLContext> OpenGLContext::create(void* window)
	{
	    return createScope<OpenGLContext>(static_cast<GLFWwindow*>(window));
	}
}
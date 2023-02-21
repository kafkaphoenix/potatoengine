#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace potatocraft {

    class OpenGLContext
    {
    public:
		OpenGLContext(GLFWwindow* window);

		void init();
		void swapBuffers();

		static Scope<OpenGLContext> create(void* window);
	private:
		GLFWwindow* m_window;
    };
 }
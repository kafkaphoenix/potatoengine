#pragma once

#include <GLFW/glfw3.h>

namespace potatoengine {

    class OpenGLContext
    {
    public:
		OpenGLContext(GLFWwindow* window);

		void init();
		void swapBuffers();

		static Scope<OpenGLContext> Create(void* window);
	private:
		GLFWwindow* m_window;
    };
 }
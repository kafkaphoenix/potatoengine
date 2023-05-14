#pragma once

#include <GLFW/glfw3.h>

#include "src/pch.h"

namespace potatoengine {

    class OpenGLContext
    {
    public:
		OpenGLContext(GLFWwindow* window);

		void init();
		void swapBuffers();

		static std::unique_ptr<OpenGLContext> Create(void* window);
	private:
		GLFWwindow* m_window;
    };
 }
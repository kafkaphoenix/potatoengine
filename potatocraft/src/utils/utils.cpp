#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include <src/utils/utils.hpp>

namespace potatocraft {

	float Time::currentTime()
	{
		return glfwGetTime();
	}
}
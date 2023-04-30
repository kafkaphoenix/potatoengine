#include "src/pch.h"
#include "src/core/window.h"

#include "src/event/applicationEvent.h"
#include "src/event/mouseEvent.h"
#include "src/event/keyEvent.h"

namespace potatoengine
{

    Window::Window(const WindowProperties& properties) {
        init(properties);
    }

    Window::~Window() {
        shutdown();
    }

    void Window::init(const WindowProperties& properties) {
        m_data.title = properties.title;
        m_data.width = properties.width;
        m_data.height = properties.height;

        fprintf(stdout, "Creating window for the app %s with resolution (%u, %u)\n", properties.title.c_str(), properties.width, properties.height);

        if (s_GLFWWindowCount == 0)
		{
            fprintf(stdout, "Starting GLFW context, OpenGL 4.6\n");
            if (!glfwInit()) {
                fprintf(stderr, "Failed to initialize GLFW!\n");
            }
            glfwSetErrorCallback([](int error, const char *description)
            {
                fprintf(stderr, "GLFW Error %d: %s\n", error,  description);
            });
        }

        const char *glsl_version = "#version 460";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_DEPTH_BITS, 24);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

        int monitorCount;
        GLFWmonitor **monitors = glfwGetMonitors(&monitorCount);

        int xpos = 50;
        int ypos = 50;
        if (monitorCount >= 2)
        {
            xpos = 500;
            ypos = 200;
        }

        m_window = glfwCreateWindow(int(properties.width), (int)properties.height, m_data.title.c_str(), nullptr, nullptr);
        ++s_GLFWWindowCount;

        glfwSetWindowMonitor(m_window, nullptr, xpos, ypos, int(properties.width), (int)properties.height, 0);
        m_context = OpenGLContext::Create(m_window);
		m_context->init();
        glViewport(0.f, 0.f, int(properties.width), (int)properties.height);

		glfwSetWindowUserPointer(m_window, &m_data);
		setVSync(true);

        glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            data.width = width;
            data.height = height;

            WindowResizeEvent event(width, height);
            data.eventCallback(event);
        });

        glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            WindowCloseEvent event;
            data.eventCallback(event);
        });

        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

        glfwSetKeyCallback(m_window, [](GLFWwindow *window, int key, int scancode, int action, int mode)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            switch (action)
            {
                case GLFW_PRESS:
                {
                    KeyPressedEvent event(key, 0);
                    data.eventCallback(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    KeyReleasedEvent event(key);
                    data.eventCallback(event);
                    break;
                }
                case GLFW_REPEAT:
                {
                    KeyPressedEvent event(key, true);
                    data.eventCallback(event);
                    break;
                }
            }
        });

        glfwSetCharCallback(m_window, [](GLFWwindow *window, uint32_t keycode)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            KeyTypedEvent event(keycode);
            data.eventCallback(event);
        });

        glfwSetMouseButtonCallback(m_window, [](GLFWwindow *window, int button, int action, int mode)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            switch (action)
            {
                case GLFW_PRESS:
                {
                    MouseButtonPressedEvent event(button);
                    data.eventCallback(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    MouseButtonReleasedEvent event(button);
                    data.eventCallback(event);
                    break;
                }
            }
        });

        glfwSetCursorPosCallback(m_window, [](GLFWwindow *window, double xPos, double yPos)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            MouseMovedEvent event((float)xPos, (float)yPos);
            data.eventCallback(event);
        });

        glfwSetScrollCallback(m_window, [](GLFWwindow *window, double xOffset, double yOffset)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            MouseScrolledEvent event((float)xOffset, (float)yOffset);
            data.eventCallback(event);
        });
    }

    void Window::shutdown() {
		glfwDestroyWindow(m_window);
		--s_GLFWWindowCount;

		if (s_GLFWWindowCount == 0)
		{
			glfwTerminate();
		}
    }

    void Window::onUpdate() {
        glfwPollEvents();
        m_context->swapBuffers();
    }

    void Window::setVSync(bool enabled) {
        glfwSwapInterval(enabled ? 1 : 0);
        m_data.vSync = enabled;        
    }

	Scope<Window> Window::Create(const WindowProperties& properties)
	{
	    return createScope<Window>(properties);
	}

}
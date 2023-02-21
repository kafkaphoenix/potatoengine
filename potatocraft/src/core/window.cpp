#include "src/pcpch.hpp"
#include "src/core/window.hpp"

#include "src/event/applicationEvent.hpp"
#include "src/event/mouseEvent.hpp"
#include "src/event/keyEvent.hpp"

namespace potatocraft
{
    static uint8_t s_GLFWWindowCount = 0;

    static void glfw_error_callback(int error, const char *description)
    {
        fprintf(stderr, "GLFW Error %d: %s\n", error,  description);
    }

    Window::Window(const WindowProps& props) {
        init(props);
    }

    Window::~Window() {
        shutdown();
    }

    void Window::init(const WindowProps& props) {
        m_data.title = props.title;
        m_data.width = props.width;
        m_data.height = props.height;

        fprintf(stdout, "Creating window %s (%u, %u).\n", props.title.c_str(), props.width, props.height);

        if (s_GLFWWindowCount == 0)
		{
            fprintf(stdout, "Starting GLFW context, OpenGL 4.6.\n");
            int success = glfwInit();
            if (success == 0) {
                fprintf(stderr, "Failed to initialize GLFW!\n");
            }
            glfwSetErrorCallback(glfw_error_callback);
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

        m_window = glfwCreateWindow(int(props.width), (int)props.height, m_data.title.c_str(), nullptr, nullptr);
        ++s_GLFWWindowCount;

        glfwSetWindowMonitor(m_window, nullptr, xpos, ypos, int(props.width), (int)props.height, 0);
        m_context = OpenGLContext::create(m_window);
		m_context->init();
        glViewport(0.f, 0.f, int(props.width), (int)props.height);

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

        glfwSetCharCallback(m_window, [](GLFWwindow *window, unsigned int keycode)
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
        if (enabled) {
            glfwSwapInterval(1);
        } else {
            glfwSwapInterval(0);
        }
 
        m_data.vSync = enabled;        
    }

	Scope<Window> Window::create(const WindowProps& props)
	{
	    return createScope<Window>(props);
	}

}
#include "src/core/window.h"

#include "src/events/applicationEvent.h"
#include "src/events/mouseEvent.h"
#include "src/events/keyEvent.h"

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
        m_data.lastX = properties.width / 2.0f;
        m_data.lastY = properties.height / 2.0f;

        fprintf(stdout, "Creating window for the app %s with resolution %ix%i\n", properties.title.c_str(), properties.width, properties.height);

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

        int xpos = (monitorCount >= 2) ? 500 : 50;
        int ypos = (monitorCount >= 2) ? 200 : 50;

        m_window = glfwCreateWindow(properties.width, properties.height, m_data.title.c_str(), nullptr, nullptr);
        ++s_GLFWWindowCount;

        glfwSetWindowMonitor(m_window, nullptr, xpos, ypos, properties.width, properties.height, 0);
        m_context = OpenGLContext::Create(m_window);
		m_context->init();
        glViewport(0.f, 0.f, properties.width, properties.height);

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

        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        glfwSetKeyCallback(m_window, [](GLFWwindow *window, int key, int scancode, int action, int mode)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            switch (action)
            {
                case GLFW_PRESS:
                {
                    KeyPressedEvent event(key, false);
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

        glfwSetCursorPosCallback(m_window, [](GLFWwindow *window, double xpos, double ypos)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            if (not data.updateCameraPosition) {
                return;
            }

            if (data.firstMouse)
            {
                data.lastX = xpos;
                data.lastY = ypos;
                data.firstMouse = false;
            }

            float xoffset = xpos - data.lastX;
            float yoffset = data.lastY - ypos; // reversed since y-coordinates go from bottom to top

            data.lastX = xpos;
            data.lastY = ypos;

            MouseMovedEvent event(xoffset, yoffset);
            data.eventCallback(event);
        });

        glfwSetScrollCallback(m_window, [](GLFWwindow *window, double xoffset, double yoffset)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            if (not data.updateCameraPosition) {
                return;
            }

            MouseScrolledEvent event(xoffset, yoffset);
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

    void Window::setCursorMode(CursorMode mode) {
        glfwSetInputMode(m_window, GLFW_CURSOR, static_cast<int>(mode));
    }

	std::unique_ptr<Window> Window::Create(const WindowProperties& properties)
	{
	    return std::make_unique<Window>(properties);
	}

}
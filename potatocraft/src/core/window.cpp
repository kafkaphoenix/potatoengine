#include "src/pcpch.hpp"
#include "src/core/window.hpp"
#include "src/debug/GLDebugMessageCallback.h" // espera REMOVE

namespace potatocraft
{
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
            fprintf(stderr, "Failed to initialize OpenGL loader!\n");
            // espera m_window->shutdown();
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

        // Set GLFW callbacks
        /* espera glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetKeyCallback(m_window, key_callback);
        glfwSetCursorPosCallback(m_window, mouse_callback);
        glfwSetScrollCallback(m_window, scroll_callback);*/

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
    
    /* espera void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
    {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window)

        keyMap.updateKeyState(key, action);
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) // TODO add menu
            glfwSetWindowShouldClose(window, GL_TRUE);
    }

    void mouse_callback(GLFWwindow *window, double xpos, double ypos)
    {
        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

        lastX = xpos;
        lastY = ypos;

        cam.processMouseMovement(xoffset, yoffset);
    }

    void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
    {
        cam.processMouseScroll(yoffset);
    }*/

	Scope<Window> Window::create(const WindowProps& props)
	{
	    return createScope<Window>(props);
	}

}
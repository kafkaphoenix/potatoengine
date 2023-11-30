#include "potatoengine/core/window.h"

#include <stb_image.h>

#include "potatoengine/events/keyEvent.h"
#include "potatoengine/events/mouseEvent.h"
#include "potatoengine/events/windowEvent.h"

namespace potatoengine {

Window::Window(WindowProperties&& properties) {
    m_data.name = std::move(properties.name);
    m_data.width = properties.windowWidth;
    m_data.height = properties.windowHeight;
    m_data.lastX = m_data.width / 2.f;
    m_data.lastY = m_data.height / 2.f;
    m_data.windowIconPath = std::move(properties.windowIconPath);
    ENGINE_INFO("Creating window for {} app with resolution {}x{}...", m_data.name, m_data.width, m_data.height);
    if (s_GLFWWindowCount == 0) {
        ENGINE_ASSERT(glfwInit(), "Failed to initialize GLFW!");
        glfwSetErrorCallback([](int error, const char* description) {
            ENGINE_ASSERT(false, "GLFW Error! {0}: {1}", error, description);
        });
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, properties.openglMajorVersion);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, properties.openglMinorVersion);
    glfwWindowHint(GLFW_DEPTH_BITS, properties.depthBits);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, properties.resizable);

    ENGINE_INFO("Loading OpengGL version {}.{}", properties.openglMajorVersion, properties.openglMinorVersion);

    int monitorCount;
    glfwGetMonitors(&monitorCount);
    GLFWmonitor* monitor = glfwGetMonitors(&monitorCount)[properties.primaryMonitor];
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    if (properties.fullscreen) {
        glfwWindowHint(GLFW_REFRESH_RATE, properties.refreshRate);
        m_data.width = mode->width;
        m_data.height = mode->height;
        m_window = glfwCreateWindow(m_data.width, m_data.height, m_data.name.data(), monitor, nullptr);
        m_data.fullscreen = true;
    } else {
        m_data.width = properties.windowWidth;
        m_data.height = properties.windowHeight;
        m_window = glfwCreateWindow(m_data.width, m_data.height, m_data.name.data(), nullptr, nullptr);
        int xpos = (mode->width - m_data.width) / 2;
        int ypos = (mode->height - m_data.height) / 2;
        setPosition(xpos, ypos);
        m_data.fullscreen = false;
    }

    ++s_GLFWWindowCount;

    m_context = OpenGLContext::Create(m_window);
    m_context->init();  // make context current window
    glViewport(0, 0, m_data.width, m_data.height);

    setVSync(properties.vSync);
    setWindowIcon(m_data.windowIconPath);
    setCursorIcon(properties.cursorIconPath);
    setCursorMode(properties.cursorMode);
    glfwSetWindowUserPointer(m_window, &m_data);

    glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int width, int height) {
        WindowData& data = *std::bit_cast<WindowData*>(glfwGetWindowUserPointer(window));
        data.width = width;
        data.height = height;

        WindowResizeEvent event(width, height);
        data.eventCallback(event);
    });

    glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window) {
        WindowData& data = *std::bit_cast<WindowData*>(glfwGetWindowUserPointer(window));
        WindowCloseEvent event;
        data.eventCallback(event);
    });

    glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int, int action, int) {
        WindowData& data = *std::bit_cast<WindowData*>(glfwGetWindowUserPointer(window));

        switch (action) {
            case GLFW_PRESS: {
                KeyPressedEvent event(key, false);
                data.eventCallback(event);
                break;
            }
            case GLFW_RELEASE: {
                KeyReleasedEvent event(key);
                data.eventCallback(event);
                break;
            }
            case GLFW_REPEAT: {
                KeyPressedEvent event(key, true);
                data.eventCallback(event);
                break;
            }
        }
    });

    glfwSetCharCallback(m_window, [](GLFWwindow* window, uint32_t keycode) {
        WindowData& data = *std::bit_cast<WindowData*>(glfwGetWindowUserPointer(window));

        KeyTypedEvent event(keycode);
        data.eventCallback(event);
    });

    glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int) {
        WindowData& data = *std::bit_cast<WindowData*>(glfwGetWindowUserPointer(window));

        switch (action) {
            case GLFW_PRESS: {
                MouseButtonPressedEvent event(button);
                data.eventCallback(event);
                break;
            }
            case GLFW_RELEASE: {
                MouseButtonReleasedEvent event(button);
                data.eventCallback(event);
                break;
            }
        }
    });

    glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xpos, double ypos) {
        WindowData& data = *std::bit_cast<WindowData*>(glfwGetWindowUserPointer(window));

        if (not data.updateCameraPosition) {
            return;
        }

        if (data.firstMouse) {
            data.lastX = (float)xpos;
            data.lastY = (float)ypos;
            data.firstMouse = false;
        }

        float xoffset = (float)xpos - data.lastX;
        float yoffset = data.lastY - (float)ypos;  // reversed since y-coordinates go from bottom to top

        data.lastX = (float)xpos;
        data.lastY = (float)ypos;

        MouseMovedEvent event(xoffset, yoffset);
        data.eventCallback(event);
    });

    glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xoffset, double yoffset) {
        WindowData& data = *std::bit_cast<WindowData*>(glfwGetWindowUserPointer(window));

        if (not data.updateCameraPosition) {
            return;
        }

        MouseScrolledEvent event((float)xoffset, (float)yoffset);
        data.eventCallback(event);
    });

    glfwSetWindowIconifyCallback(m_window, [](GLFWwindow* window, int iconified) {
        WindowData& data = *std::bit_cast<WindowData*>(glfwGetWindowUserPointer(window));

        if (iconified) {
            data.minimized = true;
            WindowMinimizedEvent event;
            data.eventCallback(event);
        } else {
            data.minimized = false;
            WindowRestoredEvent event;
            data.eventCallback(event);
        }
    });

    glfwSetWindowMaximizeCallback(m_window, [](GLFWwindow* window, int maximized) {
        WindowData& data = *std::bit_cast<WindowData*>(glfwGetWindowUserPointer(window));

        if (maximized) {
            data.maximized = true;
            WindowMaximizedEvent event;
            data.eventCallback(event);
        } else {
            data.maximized = false;
            WindowRestoredEvent event;
            data.eventCallback(event);
        }
    });

    glfwSetWindowFocusCallback(m_window, [](GLFWwindow* window, int focused) {
        WindowData& data = *std::bit_cast<WindowData*>(glfwGetWindowUserPointer(window));

        if (focused) {
            WindowFocusEvent event;
            data.eventCallback(event);
        } else {
            WindowLostFocusEvent event;
            data.eventCallback(event);
        }
    });

    glfwSetWindowPosCallback(m_window, [](GLFWwindow* window, int xpos, int ypos) {
        WindowData& data = *std::bit_cast<WindowData*>(glfwGetWindowUserPointer(window));

        WindowMovedEvent event(xpos, ypos);
        data.eventCallback(event);
    });
}

Window::~Window() {
    ENGINE_WARN("Deleting window");
    shutdown();
}

void Window::shutdown() noexcept {
    glfwDestroyWindow(m_window);
    --s_GLFWWindowCount;

    if (s_GLFWWindowCount == 0) {
        ENGINE_WARN("No more windows! Terminating GLFW");
        glfwTerminate();
    }
}

void Window::onEvent() noexcept {
    glfwPollEvents();
}

void Window::onUpdate() noexcept {
    m_context->swapBuffers();
}

void Window::setWindowTitle(const std::string& title) {
    glfwSetWindowTitle(m_window, title.data());
}

void Window::setWindowIcon(const std::string& path) {
    GLFWimage images[1];
    images[0].pixels = stbi_load(path.data(), &images[0].width, &images[0].height, 0, 4);
    glfwSetWindowIcon(m_window, 1, images);
    stbi_image_free(images[0].pixels);
}

void Window::restoreWindowIcon() {
    if (not m_data.windowIconPath.empty()) {
        setWindowIcon(m_data.windowIconPath);
    }
}

void Window::setWindowMonitor(int monitor) {
    glfwSetWindowMonitor(m_window, glfwGetMonitors(nullptr)[monitor], 0, 0, m_data.width, m_data.height, GLFW_DONT_CARE);
}

void Window::setVSync(bool enabled) {
    glfwSwapInterval(enabled ? 1 : 0);
    m_data.vSync = enabled;
}

void Window::setCursorIcon(const std::string& path) {
    GLFWimage images[1];
    images[0].pixels = stbi_load(path.data(), &images[0].width, &images[0].height, 0, 4);
    m_data.cursor = glfwCreateCursor(&images[0], 0, 0);
    glfwSetCursor(m_window, m_data.cursor);
    stbi_image_free(images[0].pixels);
}

void Window::setCursorMode(CursorMode mode) {
    if (mode == CursorMode::Normal) {
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    } else if (mode == CursorMode::Hidden) {
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    } else if (mode == CursorMode::Disabled) {
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    } else {
        ENGINE_ASSERT(false, "Invalid cursor mode!");
    }
    m_data.cursorMode = mode;
}

void Window::restoreCursor() {
    if (m_data.cursor) {
        glfwSetCursor(m_window, m_data.cursor);
    }
}

void Window::setResizable(bool resizable) {
    glfwSetWindowAttrib(m_window, GLFW_RESIZABLE, resizable);
}

void Window::setRefreshRate(int refreshRate) {
    // This only works for fullscreen windows
    if (not m_data.fullscreen) {
        ENGINE_ERROR("Cannot set refresh rate of windowed window!");
        return;
    }

    if (refreshRate not_eq m_data.refreshRate) {
        int monitorCount;
        glfwGetMonitors(&monitorCount);
        GLFWmonitor* monitor = glfwGetMonitors(&monitorCount)[m_data.primaryMonitor];
        glfwSetWindowMonitor(m_window, monitor, 0, 0, m_data.width, m_data.height, refreshRate);
        m_data.refreshRate = refreshRate;
    }
}

void Window::setSize(int width, int height) {
    // For fullscreen windows it updates the resolution
    glfwSetWindowSize(m_window, width, height);
    m_data.width = width;
    m_data.height = height;
    glViewport(0, 0, m_data.width, m_data.height);
}

void Window::setPosition(int x, int y) {
    // This only works for windowed windows
    if (m_data.fullscreen) {
        ENGINE_ERROR("Cannot set position of fullscreen window!");
        return;
    }

    glfwSetWindowPos(m_window, x, y);
}

void Window::minimize(bool minimize) {
    // TODO check with several windows if this stops application
    if (minimize) {
        glfwIconifyWindow(m_window);
    } else {
        glfwRestoreWindow(m_window);
    }
}

void Window::maximize(bool maximize) {
    // This only works for windowed windows
    if (m_data.fullscreen) {
        ENGINE_ERROR("Cannot maximize fullscreen window!");
        return;
    }

    if (maximize) {
        glfwMaximizeWindow(m_window);
    } else {
        glfwRestoreWindow(m_window);
    }
}

void Window::setFocus(bool focused) {
    if (focused) {
        glfwFocusWindow(m_window);
    } else {
        glfwFocusWindow(nullptr);
    }
}

void Window::setFullscreen(bool fullscreen) {
    int monitorCount;
    glfwGetMonitors(&monitorCount);
    GLFWmonitor* monitor = glfwGetMonitors(&monitorCount)[m_data.primaryMonitor];
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    if (fullscreen) {
        m_data.width = mode->width;
        m_data.height = mode->height;
        glfwSetWindowMonitor(m_window, monitor, 0, 0, m_data.width, m_data.height, m_data.refreshRate);
    } else {
        m_data.width = mode->width / 2;
        m_data.height = mode->height / 2;
        int xpos = (mode->width - m_data.width) / 2;
        int ypos = (mode->height - m_data.height) / 2;
        glfwSetWindowMonitor(m_window, nullptr, xpos, ypos, m_data.width, m_data.height, GLFW_DONT_CARE);
        restoreWindowIcon();
    }
    m_data.fullscreen = fullscreen;
    glViewport(0, 0, m_data.width, m_data.height);
}

void Window::setVisible(bool visible) {
    // TODO check with application pause
    if (visible) {
        glfwShowWindow(m_window);
    } else {
        glfwHideWindow(m_window);
    }
}

std::unique_ptr<Window> Window::Create(WindowProperties&& properties) {
    return std::make_unique<Window>(std::move(properties));
}
}
#include "core/windowsManager.h"

#include <stb_image.h>

#include "core/application.h"
#include "events/keyEvent.h"
#include "events/mouseEvent.h"
#include "events/windowEvent.h"

namespace potatoengine {

WindowsManager::WindowsManager(
  const std::unique_ptr<SettingsManager>& settings_manager) {
  m_data.windowTitle = settings_manager->appName;
  m_data.width = settings_manager->windowWidth;
  m_data.height = settings_manager->windowHeight;
  m_data.primaryMonitor = settings_manager->primaryMonitor;
  m_data.mouseX = m_data.width / 2.0f;  // center of the screen
  m_data.mouseY = m_data.height / 2.0f; // center of the screen
  m_data.windowInsideImgui = settings_manager->windowInsideImgui;
  m_data.fitToWindow = settings_manager->fitToWindow;
  ENGINE_TRACE("Creating window {} with resolution {}x{}...",
               m_data.windowTitle, m_data.width, m_data.height);
  if (s_GLFWWindowCount == 0) {
    ENGINE_ASSERT(glfwInit(), "Failed to initialize GLFW!");
    glfwSetErrorCallback([](int error, const char* description) {
      ENGINE_ASSERT(false, "GLFW Error! {0}: {1}", error, description);
    });
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,
                 settings_manager->openglMajorVersion);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,
                 settings_manager->openglMinorVersion);
  glfwWindowHint(GLFW_DEPTH_BITS, settings_manager->depthBits);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, settings_manager->resizable);
  m_data.resizable = settings_manager->resizable;
  glfwWindowHint(GLFW_REFRESH_RATE, settings_manager->refreshRate);
  m_data.refreshRate = settings_manager->refreshRate;

  ENGINE_TRACE("Loading OpengGL version {}.{}",
               settings_manager->openglMajorVersion,
               settings_manager->openglMinorVersion);

  int monitorCount;
  GLFWmonitor** monitors = glfwGetMonitors(&monitorCount);
  if (settings_manager->primaryMonitor < 0 or
      settings_manager->primaryMonitor >= monitorCount) {
    ENGINE_ERROR("Invalid monitor index!");
    return;
  }
  const GLFWvidmode* mode =
    glfwGetVideoMode(monitors[settings_manager->primaryMonitor]);
  int xpos = (mode->width - m_data.width) / 2;
  int ypos = (mode->height - m_data.height) / 2;
  if (settings_manager->fullscreen) {
    m_data.fullscreen = true;
    m_window =
      glfwCreateWindow(mode->width, mode->height, m_data.windowTitle.c_str(),
                       monitors[settings_manager->primaryMonitor], nullptr);
    // center of the screen to avoid 0 0 when windowed first time
    m_data.positionX = xpos;
    m_data.positionY = ypos;
  } else {
    m_data.fullscreen = false;
    m_window = glfwCreateWindow(m_data.width, m_data.height,
                                m_data.windowTitle.c_str(), nullptr, nullptr);
    setPosition(xpos, ypos);
  }

  ++s_GLFWWindowCount;

  m_context = OpenGLContext::Create(m_window);
  m_context->init(); // make context current window
  if (m_data.fullscreen) {
    glViewport(0, 0, mode->width, mode->height);
  } else {
    glViewport(0, 0, m_data.width, m_data.height);
  }

  setWindowIcon(settings_manager->windowIconPath);
  setCursorMode(static_cast<CursorMode>(settings_manager->cursorMode), true);
  setCursorIcon(settings_manager->cursorIconPath);
  toggleVSync(settings_manager->vSync);
  glfwSetWindowUserPointer(m_window, &m_data);

  glfwSetFramebufferSizeCallback(
    m_window, [](GLFWwindow* window, int width, int height) {
      WindowData& data =
        *std::bit_cast<WindowData*>(glfwGetWindowUserPointer(window));

      if (not data.fullscreen) {
        data.width = width;
        data.height = height;
      }

      // for fullscreen it will update the resolution
      events::WindowResizeEvent event(width, height);
      data.eventCallback(event);
    });

  glfwSetWindowPosCallback(
    m_window, [](GLFWwindow* window, int xpos, int ypos) {
      WindowData& data =
        *std::bit_cast<WindowData*>(glfwGetWindowUserPointer(window));

      if (not data.fullscreen) {
        data.positionX = xpos;
        data.positionY = ypos;
        events::WindowMovedEvent event(xpos, ypos);
        data.eventCallback(event);
      }
    });

  glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window) {
    WindowData& data =
      *std::bit_cast<WindowData*>(glfwGetWindowUserPointer(window));
    events::WindowCloseEvent event;
    data.eventCallback(event);
  });

  glfwSetKeyCallback(
    m_window, [](GLFWwindow* window, int key, int, int action, int) {
      WindowData& data =
        *std::bit_cast<WindowData*>(glfwGetWindowUserPointer(window));

      switch (action) {
      case GLFW_PRESS: {
        events::KeyPressedEvent event(key, false);
        data.eventCallback(event);
        break;
      }
      case GLFW_RELEASE: {
        events::KeyReleasedEvent event(key);
        data.eventCallback(event);
        break;
      }
      case GLFW_REPEAT: {
        events::KeyPressedEvent event(key, true);
        data.eventCallback(event);
        break;
      }
      }
    });

  glfwSetCharCallback(m_window, [](GLFWwindow* window, uint32_t keycode) {
    WindowData& data =
      *std::bit_cast<WindowData*>(glfwGetWindowUserPointer(window));

    events::KeyTypedEvent event(keycode);
    data.eventCallback(event);
  });

  glfwSetMouseButtonCallback(
    m_window, [](GLFWwindow* window, int button, int action, int) {
      WindowData& data =
        *std::bit_cast<WindowData*>(glfwGetWindowUserPointer(window));

      switch (action) {
      case GLFW_PRESS: {
        events::MouseButtonPressedEvent event(button);
        data.eventCallback(event);
        break;
      }
      case GLFW_RELEASE: {
        events::MouseButtonReleasedEvent event(button);
        data.eventCallback(event);
        break;
      }
      }
    });

  glfwSetCursorPosCallback(
    m_window, [](GLFWwindow* window, double xpos, double ypos) {
      WindowData& data =
        *std::bit_cast<WindowData*>(glfwGetWindowUserPointer(window));

      if (data.firstMouse) {
        data.debugMouseX = (float)xpos;
        data.debugMouseY = (float)ypos;
        data.firstMouse = false;
      }

      float xoffset = (float)xpos - data.debugMouseX;
      float yoffset =
        data.debugMouseY -
        (float)ypos; // reversed since y-coordinates go from bottom to top

      data.debugMouseX = (float)xpos;
      data.debugMouseY = (float)ypos;

      if (not data.toggleCameraPositionUpdate) {
        return;
      }

      data.mouseX = data.debugMouseX;
      data.mouseY = data.debugMouseY;

      events::MouseMovedEvent event(xoffset, yoffset);
      data.eventCallback(event);
    });

  glfwSetScrollCallback(
    m_window, [](GLFWwindow* window, double xoffset, double yoffset) {
      WindowData& data =
        *std::bit_cast<WindowData*>(glfwGetWindowUserPointer(window));

      if (not data.toggleCameraPositionUpdate) {
        return;
      }

      events::MouseScrolledEvent event((float)xoffset, (float)yoffset);
      data.eventCallback(event);
    });

  glfwSetWindowIconifyCallback(m_window, [](GLFWwindow* window, int minimized) {
    WindowData& data =
      *std::bit_cast<WindowData*>(glfwGetWindowUserPointer(window));

    if (minimized == GLFW_TRUE) {
      data.minimized = true;
      events::WindowMinimizedEvent event;
      data.eventCallback(event);
    } else {
      data.minimized = false;
      events::WindowRestoredEvent event;
      data.eventCallback(event);
    }
  });

  glfwSetWindowMaximizeCallback(
    m_window, [](GLFWwindow* window, int maximized) {
      WindowData& data =
        *std::bit_cast<WindowData*>(glfwGetWindowUserPointer(window));

      if (maximized == GLFW_TRUE) {
        data.maximized = true;
        events::WindowMaximizedEvent event;
        data.eventCallback(event);
      } else {
        data.maximized = false;
        events::WindowRestoredEvent event;
        data.eventCallback(event);
      }
    });

  glfwSetWindowFocusCallback(m_window, [](GLFWwindow* window, int focused) {
    WindowData& data =
      *std::bit_cast<WindowData*>(glfwGetWindowUserPointer(window));

    if (focused == GLFW_TRUE) {
      data.focused = true;
      events::WindowFocusEvent event;
      data.eventCallback(event);
    } else {
      data.focused = false;
      events::WindowLostFocusEvent event;
      data.eventCallback(event);
    }
  });
}

WindowsManager::~WindowsManager() {
  ENGINE_WARN("Deleting window");
  shutdown();
}

void WindowsManager::shutdown() {
  glfwDestroyWindow(m_window);
  --s_GLFWWindowCount;

  if (s_GLFWWindowCount == 0) {
    ENGINE_WARN("No more windows! Terminating GLFW");
    glfwTerminate();
  }
}

void WindowsManager::onUpdate() {
  m_context->swapBuffers();
  glfwPollEvents();
}

void WindowsManager::triggerEvent(events::Event& e) { m_data.eventCallback(e); }

void WindowsManager::setPosition(int x, int y) {
  if (Application::Get().getSettingsManager()->fullscreen) {
    ENGINE_ERROR("Cannot set position of fullscreen window!");
    return;
  }

  if (x not_eq m_data.positionX or y not_eq m_data.positionY) {
    glfwSetWindowPos(m_window, x, y);
    m_data.positionX = x;
    m_data.positionY = y;
  }
}

void WindowsManager::setLastMousePosition(float x, float y) {
  m_data.mouseX = x;
  m_data.mouseY = y;
}

void WindowsManager::toggleCameraPositionUpdate(bool enable) {
  m_data.toggleCameraPositionUpdate = enable;
}

void WindowsManager::minimize(bool minimize) {
  if (minimize and not m_data.minimized) {
    glfwIconifyWindow(m_window);
  } else if (not minimize and m_data.minimized) {
    glfwRestoreWindow(m_window);
  }
}

void WindowsManager::maximize(bool maximize) {
  if (Application::Get().getSettingsManager()->fullscreen) {
    ENGINE_ERROR("Cannot maximize fullscreen window!");
    return;
  }

  if (maximize and not m_data.maximized) {
    glfwMaximizeWindow(m_window);
  } else if (not maximize and m_data.maximized) {
    glfwRestoreWindow(m_window);
  }
}

void WindowsManager::toggleFocus(bool focused) {
  if (Application::Get().getSettingsManager()->fullscreen) {
    ENGINE_ERROR("Cannot set focus of fullscreen window!");
    return;
  }

  if (not m_data.visible) {
    ENGINE_ERROR("Cannot set focus of invisible window!");
    return;
  }

  if (m_data.minimized) {
    ENGINE_ERROR("Cannot set focus of minimized window!");
    return;
  }

  if (focused and not m_data.focused) {
    glfwFocusWindow(m_window);
  } else if (not focused and m_data.focused) {
    glfwFocusWindow(nullptr);
  }
}

void WindowsManager::toggleVisibility(bool visible) {
  if (visible and m_data.visible) {
    ENGINE_ERROR("Cannot show visible window!");
    return;
  }

  if (not visible and not m_data.visible) {
    ENGINE_ERROR("Cannot hide invisible window!");
    return;
  }

  if (visible and not m_data.visible) {
    glfwShowWindow(m_window);
    m_data.visible = visible;
  } else if (not visible and m_data.visible) {
    glfwHideWindow(m_window);
    m_data.visible = visible;
  }
}

void WindowsManager::toggleWireframe(bool wireframe) {
  if (wireframe not_eq m_data.wireframe) {
    m_data.wireframe = wireframe;
  }
}

void WindowsManager::setEventCallback(EventCallbackFn&& cb) {
  m_data.eventCallback = std::move(cb);
}

void WindowsManager::setWindowTitle(std::string title) {
  if (title.empty()) {
    ENGINE_ERROR("WindowsManager title is empty!");
    return;
  }

  if (title not_eq m_data.windowTitle) {
    glfwSetWindowTitle(m_window, title.c_str());
    m_data.windowTitle = title;
    Application::Get().getSettingsManager()->appName = title;
  }
}

void WindowsManager::setWindowIcon(std::string path) {
  if (path.empty()) {
    ENGINE_ERROR("WindowsManager icon path is empty!");
  }

  if (not std::filesystem::exists(path)) {
    ENGINE_ERROR("WindowsManager icon path {} does not exist!", path);
    return;
  }

  if (path not_eq m_data.windowIconPath) {
    GLFWimage images[1];
    images[0].pixels =
      stbi_load(path.c_str(), &images[0].width, &images[0].height, 0, 4);
    glfwSetWindowIcon(m_window, 1, images);
    stbi_image_free(images[0].pixels);
    m_data.windowIconPath = path;
    Application::Get().getSettingsManager()->windowIconPath = path;
  }
}

void WindowsManager::restoreWindowIcon() {
  if (m_data.windowIconPath.empty()) {
    ENGINE_ERROR("WindowsManager icon path is empty!");
    return;
  }

  setWindowIcon(m_data.windowIconPath);
}

void WindowsManager::setCursorIcon(std::string path) {
  if (path.empty()) {
    ENGINE_ERROR("Cursor icon path is empty!");
    return;
  }

  if (not std::filesystem::exists(path)) {
    ENGINE_ERROR("Cursor icon path {} does not exist!", path);
    return;
  }

  if (path not_eq m_data.cursorIconPath) {
    if (m_data.cursor not_eq nullptr) {
      glfwDestroyCursor(m_data.cursor);
    }

    GLFWimage images[1];
    images[0].pixels =
      stbi_load(path.c_str(), &images[0].width, &images[0].height, 0, 4);
    m_data.cursor = glfwCreateCursor(&images[0], 0, 0);
    glfwSetCursor(m_window, m_data.cursor);
    stbi_image_free(images[0].pixels);
    m_data.cursorIconPath = path;
    Application::Get().getSettingsManager()->cursorIconPath = path;
  }
}

void WindowsManager::setCursorMode(CursorMode cursorMode, bool update) {
  if (cursorMode not_eq m_data.cursorMode) {
    int mode = static_cast<int>(cursorMode);
    if (mode == 0) {
      glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    } else if (mode == 1) {
      glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    } else if (mode == 2) {
      glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    } else {
      ENGINE_ERROR("Invalid cursor mode!");
      return;
    }
    if (update) {
      m_data.cursorMode = cursorMode;
      Application::Get().getSettingsManager()->cursorMode = mode;
    }
  }
}

void WindowsManager::restoreCursor() {
  int mode = static_cast<int>(m_data.cursorMode);
  if (mode == 0) {
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  } else if (mode == 1) {
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
  } else if (mode == 2) {
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  } else {
    ENGINE_ERROR("Invalid cursor mode!");
    return;
  }

  if (m_data.cursor) { // restore cursor icon
    glfwSetCursor(m_window, m_data.cursor);
  }
}

void WindowsManager::resize(int width, int height) {
  // for fullscreen windows it updates the resolution
  // m_data is updated in the callback
  // and settings in the event
  if (width < 0 or height < 0) {
    ENGINE_ERROR("Invalid window size!");
    return;
  }
  if (width not_eq m_data.width or height not_eq m_data.height) {
    glfwSetWindowSize(m_window, width, height);
  }
}

void WindowsManager::toggleResizable(bool resizable) {
  if (resizable not_eq m_data.resizable) {
    if (m_data.fullscreen) {
      ENGINE_ERROR("Cannot set resizable of fullscreen window!");
      return;
    }

    glfwSetWindowAttrib(m_window, GLFW_RESIZABLE, resizable);
    m_data.resizable = resizable;
    Application::Get().getSettingsManager()->resizable = resizable;
  }
}

void WindowsManager::setRefreshRate(int refreshRate) {
  if (refreshRate not_eq m_data.refreshRate) {
    if (not m_data.fullscreen) {
      ENGINE_ERROR("Cannot set refresh rate of windowed window!");
      return;
    }

    if (refreshRate < 0) {
      ENGINE_ERROR("Invalid refresh rate!");
      return;
    }

    const auto& settings_manager = Application::Get().getSettingsManager();
    int monitorCount;
    GLFWmonitor* monitor =
      glfwGetMonitors(&monitorCount)[settings_manager->primaryMonitor];
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    glfwSetWindowMonitor(m_window, monitor, GLFW_DONT_CARE, GLFW_DONT_CARE,
                         mode->width, mode->height, refreshRate);
    m_data.refreshRate = refreshRate;
    settings_manager->refreshRate = refreshRate;
    if (m_data.vSync) {
      glfwSwapInterval(1); // https://github.com/glfw/glfw/issues/1072
    }
  }
}

void WindowsManager::toggleVSync(bool enabled) {
  if (enabled not_eq m_data.vSync) {
    glfwSwapInterval(enabled ? 1 : 0);
    m_data.vSync = enabled;
    Application::Get().getSettingsManager()->vSync = enabled;
  }
}

void WindowsManager::setWindowMonitor(int monitor) {
  if (monitor not_eq m_data.primaryMonitor) {
    int monitorCount;
    GLFWmonitor** monitors = glfwGetMonitors(&monitorCount);
    if (monitor < 0 or monitor >= monitorCount) {
      ENGINE_ERROR("Invalid monitor index!");
      return;
    }

    const auto& settings_manager = Application::Get().getSettingsManager();
    int xpos = m_data.fullscreen ? GLFW_DONT_CARE : m_data.positionX;
    int ypos = m_data.fullscreen ? GLFW_DONT_CARE : m_data.positionY;
    int refreshRate =
      m_data.fullscreen ? settings_manager->refreshRate : GLFW_DONT_CARE;

    glfwSetWindowMonitor(m_window, monitors[monitor], xpos, ypos, m_data.width,
                         m_data.height, refreshRate);
    m_data.primaryMonitor = monitor;
    settings_manager->primaryMonitor = monitor;
  }
}

void WindowsManager::toggleFullscreen(bool fullscreen) {
  if (fullscreen and not m_data.fullscreen) {
    const auto& settings_manager = Application::Get().getSettingsManager();
    int monitorCount;
    GLFWmonitor* monitor =
      (glfwGetMonitors(&monitorCount))[settings_manager->primaryMonitor];
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    m_data.fullscreen = fullscreen;
    settings_manager->fullscreen = fullscreen;
    glfwSetWindowMonitor(m_window, monitor, GLFW_DONT_CARE, GLFW_DONT_CARE,
                         mode->width, mode->height,
                         settings_manager->refreshRate);
    if (m_data.vSync) {
      glfwSwapInterval(1); // https://github.com/glfw/glfw/issues/1072
    }
  } else if (not fullscreen and m_data.fullscreen) {
    m_data.fullscreen = fullscreen;
    Application::Get().getSettingsManager()->fullscreen = fullscreen;
    glfwSetWindowMonitor(m_window, nullptr, m_data.positionX, m_data.positionY,
                         m_data.width, m_data.height, GLFW_DONT_CARE);
    restoreWindowIcon();
    if (m_data.vSync) {
      glfwSwapInterval(1); // https://github.com/glfw/glfw/issues/1072
    }
  }
}

void WindowsManager::toggleWindowInsideImgui(bool windowInsideImgui) {
  if (windowInsideImgui not_eq m_data.windowInsideImgui) {
    m_data.windowInsideImgui = windowInsideImgui;
    Application::Get().getSettingsManager()->windowInsideImgui =
      windowInsideImgui;
  }
}

void WindowsManager::toggleFitToWindow(bool fitToWindow) {
  if (fitToWindow not_eq m_data.fitToWindow) {
    m_data.fitToWindow = fitToWindow;
    Application::Get().getSettingsManager()->fitToWindow = fitToWindow;
  }
}

std::unique_ptr<WindowsManager> WindowsManager::Create(
  const std::unique_ptr<SettingsManager>& settings_manager) {
  return std::make_unique<WindowsManager>(settings_manager);
}
}
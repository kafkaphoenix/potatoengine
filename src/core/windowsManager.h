#pragma once

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "events/event.h"
#include "pch.h"
#include "render/openGLContext.h"
#include "core/settingsManager.h"

namespace potatoengine {

using EventCallbackFn = std::function<void(events::Event&)>;

enum class CursorMode { Normal = 0, Hidden, Disabled };

struct WindowData {
    int positionX;
    int positionY;
    float mouseX;
    float mouseY;
    float debugMouseX;
    float debugMouseY;
    bool firstMouse{true};
    bool toggleCameraPositionUpdate{true};
    bool minimized{};
    bool maximized{};
    bool focused{};
    bool visible{};
    bool wireframe{};
    EventCallbackFn eventCallback;

    // settings
    std::string windowTitle;
    std::string windowIconPath;
    int width;
    int height;
    int primaryMonitor;
    bool vSync{};
    std::string cursorIconPath;
    GLFWcursor* cursor;
    CursorMode cursorMode;
    bool resizable{};
    int refreshRate;
    bool fullscreen{};
    bool windowInsideImgui{};
    bool fitToWindow{};
};

static uint32_t s_GLFWWindowCount{};

class WindowsManager {
  public:
    WindowsManager(const std::unique_ptr<SettingsManager>& settings_manager);
    ~WindowsManager();

    void shutdown();
    void onUpdate();
    void triggerEvent(events::Event& e);

    GLFWwindow* getNativeWindow() const noexcept { return m_window; }
    const WindowData& getWindowData() const noexcept { return m_data; }

    void setPosition(int x, int y);
    void setLastMousePosition(float x, float y);
    void toggleCameraPositionUpdate(bool enable);
    void minimize(bool minimize);
    void maximize(bool maximize);
    void toggleFocus(bool focused);
    void toggleVisibility(bool visible);
    void toggleWireframe(bool wireframe);
    void setEventCallback(EventCallbackFn&& cb);

    void setWindowTitle(std::string title);
    void setWindowIcon(std::string path);
    void restoreWindowIcon();
    void setCursorIcon(std::string path);
    void setCursorMode(CursorMode mode, bool update);
    void restoreCursor();
    void resize(int width, int height);
    void toggleResizable(bool resizable);
    void setRefreshRate(int refreshRate);
    void toggleVSync(bool enabled);
    void setWindowMonitor(int monitor);
    void toggleFullscreen(bool fullscreen);
    void toggleWindowInsideImgui(bool windowInsideImgui);
    void toggleFitToWindow(bool fitToWindow);

    static std::unique_ptr<WindowsManager>
    Create(const std::unique_ptr<SettingsManager>& settings_manager);

  private:
    GLFWwindow* m_window{}; // TODO: this class should be a window manager, and
                            // this should be a vector of windows
    std::unique_ptr<OpenGLContext> m_context;
    WindowData m_data{};
};

}
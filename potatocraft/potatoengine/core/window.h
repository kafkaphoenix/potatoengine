#pragma once

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "potatoengine/events/event.h"
#include "potatoengine/pch.h"
#include "potatoengine/renderer/openGLContext.h"

namespace potatoengine {

struct WindowProperties {
    std::string title{};
    int width{};
    int height{};
};

enum class CursorMode {
    Normal = GLFW_CURSOR_NORMAL,
    Hidden = GLFW_CURSOR_HIDDEN,
    Disabled = GLFW_CURSOR_DISABLED
};

static uint8_t s_GLFWWindowCount = 0;

class Window {
   public:
    using EventCallbackFn = std::function<void(Event&)>;

    Window(WindowProperties&& properties);
    ~Window();

    void shutdown() noexcept;
    void onUpdate() noexcept;

    void setEventCallback(const EventCallbackFn& cb) { m_data.eventCallback = cb; }
    void setVSync(bool enabled) noexcept;
    bool isVSync() const noexcept { return m_data.vSync; }

    GLFWwindow* getNativeWindow() const noexcept { return m_window; }

    int getWidth() const noexcept { return m_data.width; }
    int getHeight() const noexcept { return m_data.height; }

    void setCursorMode(CursorMode mode) const noexcept;
    void setUpdateCameraPosition(bool update) noexcept { m_data.updateCameraPosition = update; }
    void setLastMousePosition(float x, float y) noexcept {
        m_data.lastX = x;
        m_data.lastY = y;
    }

    static std::unique_ptr<Window> Create(WindowProperties&& properties);
    static constexpr uint32_t OPENGL_MAYOR_VERSION = 4;
    static constexpr uint32_t OPENGL_MINOR_VERSION = 5;

   private:
    GLFWwindow* m_window{}; // TODO: this class should be a window manager, and this should be a vector of windows
    std::unique_ptr<OpenGLContext> m_context;

    struct WindowData {
        std::string title{};
        int width{}, height{};
        float lastX{};
        float lastY{};
        bool firstMouse = true;
        bool vSync{};
        CursorMode cursorMode = CursorMode::Disabled;
        bool updateCameraPosition = true;

        EventCallbackFn eventCallback;
    };

    WindowData m_data{};

};

}
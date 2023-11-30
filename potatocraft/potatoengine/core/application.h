#pragma once

#include "potatoengine/assets/assetsManager.h"
#include "potatoengine/core/state.h"
#include "potatoengine/core/stateStack.h"
#include "potatoengine/core/window.h"
#include "potatoengine/events/event.h"
#include "potatoengine/pch.h"
#include "potatoengine/renderer/renderer.h"

int main(int argc, char** argv);

namespace potatoengine {

struct Config {
    std::string name{};
    std::string root{};
    std::string windowIconPath{};
    int windowWidth{};
    int windowHeight{};
    int depthBits{};
    int refreshRate{};
    bool fullscreen{};
    int primaryMonitor{};
    bool vSync{};
    bool resizable{};
    int openglMajorVersion{};
    int openglMinorVersion{};
    std::string cursorIconPath{};
    CursorMode cursorMode{};
};
struct CLArgs {
    std::span<const char*> args{};

    const char* operator[](int idx) const { return args[idx]; }
};

class Application {
   public:
    Application(Config&& c, CLArgs&& args);
    virtual ~Application();

    void onEvent(Event& e);
    void pushState(std::unique_ptr<State>&& s);
    void pushOverlay(std::unique_ptr<State>&& s);

    Window& getWindow() const noexcept { return *m_window; }  // todo rename to getActiveWindow when multi-window support is added

    void close() noexcept { m_running = false; }
    void pause(bool paused) noexcept { m_paused = paused; }
    void debug(bool debugging) noexcept { m_debugging = debugging; }

    bool isPaused() const noexcept { return m_paused; }
    bool isDebugging() const noexcept { return m_debugging; }

    static Application& Get() noexcept { return *s_instance; }

   protected:
    std::shared_ptr<AssetsManager> m_assetsManager;
    std::shared_ptr<Renderer> m_renderer;

   private:
    void run();

    std::unique_ptr<Window> m_window;
    std::unique_ptr<StateStack> m_states;

    std::string m_name{};
    bool m_running{true};
    bool m_paused{};
    bool m_debugging{};
    float m_lastFrame{};
    float m_accumulator{};

    CLArgs m_clargs{};

    inline static Application* s_instance;
    friend int ::main(int argc, char** argv);
};

// To be defined in CLIENT
Application* CreateApp(CLArgs&& args);
}
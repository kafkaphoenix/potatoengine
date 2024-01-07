#pragma once

#include "assets/assetsManager.h"
#include "core/state.h"
#include "core/stateStack.h"
#include "core/window.h"
#include "events/event.h"
#include "pch.h"
#include "renderer/renderer.h"
#include "scene/sceneManager.h"
#include "settings.h"

int main(int argc, char** argv);

namespace potatoengine {

struct CLArgs {
    std::span<const char*> args{};

    const char* operator[](int idx) const { return args[idx]; }
};

class Application {
  public:
    Application(std::unique_ptr<Settings>&& settings, CLArgs&& args);
    virtual ~Application();

    void onEvent(Event& e);
    void pushState(std::unique_ptr<State>&& s);
    void pushOverlay(std::unique_ptr<State>&& s);

    Window& getWindow() const {
      return *m_window;
    } // todo rename to getActiveWindow when multi-window support is added
    const std::unique_ptr<SceneManager>& getSceneManager() const {
      return m_sceneManager;
    }
    const std::unique_ptr<AssetsManager>& getAssetsManager() const {
      return m_assetsManager;
    }
    const std::unique_ptr<Renderer>& getRenderer() const { return m_renderer; }
    const std::unique_ptr<Settings>& getSettings() const { return m_settings; }

    void close() noexcept { m_running = false; }
    void pause(bool paused) noexcept { m_paused = paused; }
    void debug(bool debugging) noexcept { m_debugging = debugging; }

    bool isPaused() const noexcept { return m_paused; }
    bool isDebugging() const noexcept { return m_debugging; }

    static Application& Get() { return *s_instance; }

  protected:
    std::unique_ptr<SceneManager> m_sceneManager;
    std::unique_ptr<AssetsManager> m_assetsManager;
    std::unique_ptr<Renderer> m_renderer;
    std::unique_ptr<Settings> m_settings;

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
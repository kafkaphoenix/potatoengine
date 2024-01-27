#pragma once

#include "assets/assetsManager.h"
#include "core/state.h"
#include "core/stateMachine.h"
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

    void onEvent(events::Event& e);
    void pushState(std::unique_ptr<State>&& s);
    void pushOverlay(std::unique_ptr<State>&& s);
    void popState(std::string_view name);
    void popOverlay(std::string_view name);

    // TODO rename to getActiveWindow when multi-window support is added and i
    // have a window manager
    const std::unique_ptr<Window>& getWindow() const { return m_window; }
    const std::unique_ptr<SceneManager>& getSceneManager() const {
      return m_sceneManager;
    }
    const std::unique_ptr<assets::AssetsManager>& getAssetsManager() const {
      return m_assetsManager;
    }
    // TODO rename both classes and methods to managers
    const std::unique_ptr<Renderer>& getRenderer() const { return m_renderer; }
    const std::unique_ptr<Settings>& getSettings() const { return m_settings; }
    const std::unique_ptr<StateMachine>& getStateMachine() const {
      return m_states;
    }

    void close() noexcept { m_running = false; }
    void pause(bool paused) noexcept { m_paused = paused; }
    void togglePauseGame(bool pause) noexcept;
    void setRestoreGamePaused(bool restore) noexcept {
      m_restoreGamePaused = restore;
    }
    void debug(bool debugging) noexcept { m_debugging = debugging; }

    bool isPaused() const noexcept { return m_paused; }
    bool isGamePaused() const noexcept { return m_gamePaused; }
    bool isRestoreGamePaused() const noexcept { return m_restoreGamePaused; }
    bool isDebugging() const noexcept { return m_debugging; }

    static Application& Get() { return *s_instance; }

  protected:
    std::unique_ptr<SceneManager> m_sceneManager;
    std::unique_ptr<assets::AssetsManager> m_assetsManager;
    std::unique_ptr<Renderer> m_renderer;
    std::unique_ptr<Settings> m_settings;

  private:
    void run();

    std::unique_ptr<Window> m_window;
    std::unique_ptr<StateMachine> m_states;

    std::string m_name{};
    bool m_running{true};
    bool m_paused{};
    bool m_gamePaused{};
    bool m_restoreGamePaused{};
    bool m_debugging{};
    float m_lastFrame{};
    float m_accumulator{};

    CLArgs m_clargs{};

    inline static Application* s_instance;
    friend int ::main(int argc, char** argv);
};

// to be defined in user app
Application* CreateApp(CLArgs&& args);
}
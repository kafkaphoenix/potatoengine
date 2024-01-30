#pragma once

#include "assets/assetsManager.h"
#include "core/WindowsManager.h"
#include "core/state.h"
#include "core/statesManager.h"
#include "events/event.h"
#include "pch.h"
#include "render/renderManager.h"
#include "scene/sceneManager.h"
#include "core/settingsManager.h"

int main(int argc, char** argv);

namespace potatoengine {

struct CLArgs {
    std::span<const char*> args;

    const char* operator[](int idx) const { return args[idx]; }
};

class Application {
  public:
    Application(std::unique_ptr<SettingsManager>&& settings_manager, CLArgs&& args);
    virtual ~Application();

    void onEvent(events::Event& e);
    void pushState(std::unique_ptr<State>&& s);
    void pushOverlay(std::unique_ptr<State>&& s);
    void popState(std::string_view name);
    void popOverlay(std::string_view name);

    // TODO add getActiveWindow when multi-window support is added and i
    // have a window manager
    const std::unique_ptr<WindowsManager>& getWindowsManager() const {
      return m_windows_manager;
    }
    const std::unique_ptr<SceneManager>& getSceneManager() const {
      return m_scene_manager;
    }
    const std::unique_ptr<assets::AssetsManager>& getAssetsManager() const {
      return m_assets_manager;
    }
    // TODO rename both classes and methods to managers
    const std::unique_ptr<RenderManager>& getRenderManager() const {
      return m_render_manager;
    }
    const std::unique_ptr<SettingsManager>& getSettingsManager() const {
      return m_settings_manager;
    }
    const std::unique_ptr<StatesManager>& getStatesManager() const {
      return m_states_manager;
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
    std::unique_ptr<SceneManager> m_scene_manager;
    std::unique_ptr<assets::AssetsManager> m_assets_manager;
    std::unique_ptr<RenderManager> m_render_manager;
    std::unique_ptr<SettingsManager> m_settings_manager;

  private:
    void run();

    std::unique_ptr<WindowsManager> m_windows_manager;
    std::unique_ptr<StatesManager> m_states_manager;

    std::string m_name;
    bool m_running{true};
    bool m_paused{};
    bool m_gamePaused{};
    bool m_restoreGamePaused{};
    bool m_debugging{};
    float m_lastFrame{};
    float m_accumulator{};

    CLArgs m_clargs;

    inline static Application* s_instance;
    friend int ::main(int argc, char** argv);
};

// to be defined in user app
Application* CreateApp(CLArgs&& args);
}
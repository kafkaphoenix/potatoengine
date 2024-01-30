#include "core/application.h"

#include "core/time.h"
#include "ui/imdebugger.h"
#include "ui/imguiAPI.h"

namespace potatoengine {

Application::Application(std::unique_ptr<SettingsManager>&& s, CLArgs&& args)
  : m_clargs(std::move(args)) {
  s_instance = this;
  m_settings_manager = std::move(s);

  m_name = m_settings_manager->appName;
  std::filesystem::current_path(m_settings_manager->root);
  m_states_manager = StatesManager::Create();
  m_assets_manager = assets::AssetsManager::Create();

  m_windows_manager = WindowsManager::Create(m_settings_manager);
  m_windows_manager->setEventCallback(BIND_EVENT(Application::onEvent));

  m_render_manager = RenderManager::Create();
  m_render_manager->init();
  ui::ImGuiAPI::Init(m_windows_manager->getNativeWindow(),
                     m_settings_manager->openglMajorVersion,
                     m_settings_manager->openglMinorVersion);
  m_scene_manager = SceneManager::Create();
}

Application::~Application() {
  ENGINE_WARN("Deleting application");
  m_render_manager->shutdown();
  ui::ImGuiAPI::Shutdown();
}

void Application::onEvent(events::Event& e) {
  for (auto it = m_states_manager->rbegin();
       it not_eq m_states_manager->rend() and not e.m_handled; ++it) {
    (*it)->onEvent(e);
  }
}

void Application::pushState(std::unique_ptr<State>&& s) {
  s->onAttach();
  m_states_manager->pushState(std::move(s));
}

void Application::pushOverlay(std::unique_ptr<State>&& s) {
  s->onAttach();
  m_states_manager->pushOverlay(std::move(s));
}

void Application::popState(std::string_view name) { m_states_manager->popState(name); }

void Application::popOverlay(std::string_view name) {
  m_states_manager->popOverlay(name);
}

void Application::togglePauseGame(bool pause) noexcept {
  if (pause and not m_gamePaused) {
    m_gamePaused = true;
  } else if (not pause and m_gamePaused) {
    m_gamePaused = false;
  }
}

void Application::run() {
  while (m_running) {
    m_windows_manager->onEvent();

    float currentFrame = (float)glfwGetTime();
    Time ts = currentFrame - m_lastFrame;
    m_lastFrame = currentFrame;
    m_accumulator += ts;

    if (not m_paused) [[likely]] {
      while (m_accumulator > ts) {
        ui::ImGuiAPI::NewFrame();
        for (auto& state : *m_states_manager) {
          state->onUpdate(ts);
          if (m_debugging) {
            ui::drawDebugger(m_settings_manager, m_assets_manager,
                             m_render_manager, m_scene_manager);
          }
          state->onImguiUpdate();
        }
        ui::ImGuiAPI::Render();
        m_accumulator -= ts;
        if (m_accumulator < 0) {
          m_accumulator = 0;
        }
      }
    }

    m_windows_manager->onUpdate();
  }
}
}
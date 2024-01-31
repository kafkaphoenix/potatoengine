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
  m_windows_manager->setEventCallback(BIND_EVENT(onEvent));

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
  m_states_manager->getCurrentState()->onEvent(e);
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
    float currentFrame = (float)glfwGetTime();
    Time ts = currentFrame - m_lastFrame;
    m_lastFrame = currentFrame;
    m_accumulator += ts;

    if (not m_minimized) [[likely]] {
      while (m_accumulator > ts) {
        // to be able to render inside an imgui window
        ui::ImGuiAPI::NewFrame();
        const auto& current_state = m_states_manager->getCurrentState();
        current_state->onUpdate(ts);
        m_scene_manager->onUpdate(ts);

        if (m_debugging) {
          ui::drawDebugger(m_settings_manager, m_assets_manager,
                           m_render_manager, m_scene_manager, m_states_manager);
        }
        current_state->onImguiUpdate();
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
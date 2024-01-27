#include "core/application.h"

#include "core/time.h"
#include "ui/imdebugger.h"
#include "ui/imguiAPI.h"

namespace potatoengine {

Application::Application(std::unique_ptr<Settings>&& s, CLArgs&& args)
  : m_clargs(std::move(args)) {
  s_instance = this;
  m_settings = std::move(s);

  m_name = m_settings->appName;
  std::filesystem::current_path(m_settings->root);
  m_states = StateMachine::Create();
  m_assetsManager = assets::AssetsManager::Create();

  m_window = Window::Create(m_settings);
  m_window->setEventCallback(BIND_EVENT(Application::onEvent));

  m_renderer = Renderer::Create();
  m_renderer->init();
  ui::ImGuiAPI::Init(m_window->getNativeWindow(),
                     m_settings->openglMajorVersion,
                     m_settings->openglMinorVersion);
  m_sceneManager = SceneManager::Create();
}

Application::~Application() {
  ENGINE_WARN("Deleting application");
  m_renderer->shutdown();
  ui::ImGuiAPI::Shutdown();
}

void Application::onEvent(events::Event& e) {
  for (auto it = m_states->rbegin();
       it not_eq m_states->rend() and not e.m_handled; ++it) {
    (*it)->onEvent(e);
  }
}

void Application::pushState(std::unique_ptr<State>&& s) {
  s->onAttach();
  m_states->pushState(std::move(s));
}

void Application::pushOverlay(std::unique_ptr<State>&& s) {
  s->onAttach();
  m_states->pushOverlay(std::move(s));
}

void Application::popState(std::string_view name) {
  m_states->popState(name);
}

void Application::popOverlay(std::string_view name) {
  m_states->popOverlay(name);
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
    m_window->onEvent();

    float currentFrame = (float)glfwGetTime();
    Time ts = currentFrame - m_lastFrame;
    m_lastFrame = currentFrame;
    m_accumulator += ts;

    if (not m_paused) [[likely]] {
      while (m_accumulator > ts) {
        ui::ImGuiAPI::NewFrame();
        for (auto& state : *m_states) {
          state->onUpdate(ts);
          if (m_debugging) {
            ui::drawDebugger(m_settings, m_assetsManager, m_renderer,
                             m_sceneManager);
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

    m_window->onUpdate();
  }
}
}
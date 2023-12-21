#include "core/application.h"

#include "core/time.h"
#include "ui/imguiAPI.h"

namespace potatoengine {

Application::Application(Config&& c, CLArgs&& args)
  : m_name(std::move(c.name)), m_clargs(std::move(args)) {
  s_instance = this;

  std::filesystem::current_path(c.root);
  m_states = StateStack::Create();
  m_assetsManager = AssetsManager::Create();
  WindowProperties windowProperties = {
    .name = m_name,
    .windowIconPath = c.windowIconPath,
    .windowSize = c.windowSize,
    .depthBits = c.depthBits,
    .refreshRate = c.refreshRate,
    .fullscreen = c.fullscreen,
    .primaryMonitor = c.primaryMonitor,
    .vSync = c.vSync,
    .resizable = c.resizable,
    .openglMajorVersion = c.openglMajorVersion,
    .openglMinorVersion = c.openglMinorVersion,
    .cursorIconPath = c.cursorIconPath,
    .cursorMode = c.cursorMode,
    .windowInsideImgui = c.windowInsideImgui,
    .fitToWindow = c.fitToWindow};
  m_window = Window::Create(std::move(windowProperties));
  m_window->setEventCallback(BIND_EVENT(Application::onEvent));

  m_renderer = Renderer::Create(m_assetsManager);
  m_renderer->init();
  ui::ImGuiAPI::Init(m_window->getNativeWindow(), c.openglMajorVersion,
                     c.openglMinorVersion);
}

Application::~Application() {
  ENGINE_WARN("Deleting Application");
  m_renderer->shutdown();
  ui::ImGuiAPI::Shutdown();
}

void Application::onEvent(Event& e) {
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
            state->onImguiUpdate();
          }
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
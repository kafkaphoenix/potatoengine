#pragma once

#include <entt/entt.hpp>

#include "engineAPI.h"

namespace demos::dispatchers {

inline bool onWindowClosed(engine::events::WindowCloseEvent& e) {
  engine::Application::Get().close();
  return true;
}

inline bool onWindowRestored(engine::events::WindowRestoredEvent& e) {
  return true;
}

inline bool onWindowMinimized(engine::events::WindowMinimizedEvent& e) {
  return true;
}

inline bool onWindowMaximized(engine::events::WindowMaximizedEvent& e) {
  return true;
}

inline bool onWindowFocus(engine::events::WindowFocusEvent& e) {
  engine::Application::Get().minimize(false);
  if (engine::Application::Get().isRestoreGamePaused()) {
    engine::Application::Get().togglePauseGame(true);
    engine::Application::Get().setRestoreGamePaused(false);
  } else {
    engine::Application::Get().togglePauseGame(false);
  }
  return true;
}

inline bool onWindowLostFocus(engine::events::WindowLostFocusEvent& e) {
  engine::Application::Get().minimize(true);
  if (engine::Application::Get().isGamePaused()) {
    engine::Application::Get().setRestoreGamePaused(true);
  } else {
    engine::Application::Get().togglePauseGame(true);
  }
  return true;
}

inline bool onWindowMoved(engine::events::WindowMovedEvent& e) { return true; }

inline bool onWindowResized(engine::events::WindowResizeEvent& e,
                            entt::registry& registry) {
  auto& app = engine::Application::Get();
  const auto& settings_manager = app.getSettingsManager();
  const auto& render_manager = app.getRenderManager();

  render_manager->onWindowResize(e.getWidth(), e.getHeight());

  if (not settings_manager->fullscreen) { // when resizing with the mouse
    settings_manager->windowWidth = e.getWidth();
    settings_manager->windowHeight = e.getHeight();
  }

  if (not render_manager->getFramebuffers().empty()) {
    entt::entity fbo = registry.view<engine::CFBO, engine::CUUID>()
                         .front(); // TODO: support more than one?
    engine::CFBO& cfbo = registry.get<engine::CFBO>(fbo);
    render_manager->deleteFramebuffer(cfbo.fbo);
    render_manager->addFramebuffer(std::string(cfbo.fbo), e.getWidth(),
                                   e.getHeight(), cfbo.attachment);
  }

  return true;
}

inline void windowDispatcher(engine::events::Event& e,
                             entt::registry& registry) {
  engine::events::EventDispatcher dispatcher(e);

  dispatcher.dispatch<engine::events::WindowCloseEvent>(
    BIND_STATIC_EVENT(onWindowClosed));
  dispatcher.dispatch<engine::events::WindowRestoredEvent>(
    BIND_STATIC_EVENT(onWindowRestored));
  dispatcher.dispatch<engine::events::WindowMinimizedEvent>(
    BIND_STATIC_EVENT(onWindowMinimized));
  dispatcher.dispatch<engine::events::WindowMaximizedEvent>(
    BIND_STATIC_EVENT(onWindowMaximized));
  dispatcher.dispatch<engine::events::WindowResizeEvent>(
    BIND_STATIC_EVENT(onWindowResized, registry));
  dispatcher.dispatch<engine::events::WindowFocusEvent>(
    BIND_STATIC_EVENT(onWindowFocus));
  dispatcher.dispatch<engine::events::WindowLostFocusEvent>(
    BIND_STATIC_EVENT(onWindowLostFocus));
  dispatcher.dispatch<engine::events::WindowMovedEvent>(
    BIND_STATIC_EVENT(onWindowMoved));
}
}
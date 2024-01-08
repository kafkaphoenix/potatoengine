#pragma once

#include <entt/entt.hpp>
#include <imgui.h>

#include "core/application.h"
#include "events/appEvent.h"
#include "events/windowEvent.h"
#include "pch.h"
#include "renderer/rendererAPI.h"
#include "scene/components/common/cUUID.h"
#include "scene/components/graphics/cFBO.h"

namespace potatoengine {

bool onWindowClosed(WindowCloseEvent& e) {
  Application::Get().close();
  return true;
}

bool onWindowRestored(WindowRestoredEvent& e) {
  Application::Get().pause(false);
  return true;
}

bool onWindowMinimized(WindowMinimizedEvent& e) {
  Application::Get().pause(true);
  return true;
}

bool onWindowMaximized(WindowMaximizedEvent& e) { return true; }

bool onWindowFocus(WindowFocusEvent& e) { return true; }

bool onWindowMoved(WindowMovedEvent& e) { return true; }

bool onWindowResized(WindowResizeEvent& e, entt::registry& registry) {
  RendererAPI::SetViewport(0, 0, e.getWidth(), e.getHeight());
  auto& app = Application::Get();
  const auto& settings = app.getSettings();
  if (not settings->fullscreen) { // when resizing with the mouse
    settings->windowWidth = e.getWidth();
    settings->windowHeight = e.getHeight();
  }

  const auto& renderer = app.getRenderer();
  if (not renderer->getFramebuffers().empty()) {
    entt::entity fbo =
      registry.view<CFBO, CUUID>().front(); // TODO: support more than one?
    CFBO& cfbo = registry.get<CFBO>(fbo);
    renderer->deleteFramebuffer(cfbo.fbo);
    renderer->addFramebuffer(std::string(cfbo.fbo), e.getWidth(), e.getHeight(),
                             cfbo.attachment);
  }

  return true;
}

bool onAppTick(AppTickEvent& e) { return true; }

bool onAppUpdate(AppUpdateEvent& e) { return true; }

bool onAppRender(AppRenderEvent& e) { return true; }

void appSystem(entt::registry& registry, Event& e) {
  EventDispatcher dispatcher(e);

  dispatcher.dispatch<WindowCloseEvent>(BIND_STATIC_EVENT(onWindowClosed));
  dispatcher.dispatch<WindowRestoredEvent>(BIND_STATIC_EVENT(onWindowRestored));
  dispatcher.dispatch<WindowMinimizedEvent>(
    BIND_STATIC_EVENT(onWindowMinimized));
  dispatcher.dispatch<WindowMaximizedEvent>(
    BIND_STATIC_EVENT(onWindowMaximized));
  dispatcher.dispatch<WindowResizeEvent>(
    BIND_STATIC_EVENT(onWindowResized, registry));
  dispatcher.dispatch<WindowFocusEvent>(BIND_STATIC_EVENT(onWindowFocus));
  dispatcher.dispatch<WindowMovedEvent>(BIND_STATIC_EVENT(onWindowMoved));

  dispatcher.dispatch<AppTickEvent>(BIND_STATIC_EVENT(onAppTick));
  dispatcher.dispatch<AppUpdateEvent>(BIND_STATIC_EVENT(onAppUpdate));
  dispatcher.dispatch<AppRenderEvent>(BIND_STATIC_EVENT(onAppRender));
}

}
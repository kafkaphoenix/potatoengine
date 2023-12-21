#pragma once

#include <entt/entt.hpp>
#include <imgui.h>

#include "core/application.h"
#include "core/input.h"
#include "events/keyEvent.h"
#include "events/mouseEvent.h"
#include "pch.h"
#include "renderer/rendererAPI.h"
#include "scene/components/camera/cActiveCamera.h"
#include "scene/components/camera/cCamera.h"
#include "scene/components/common/cUUID.h"
#include "scene/components/physics/cTransform.h"

namespace potatoengine {

bool onMouseMoved(MouseMovedEvent& e, entt::registry& r) {
  ImGuiIO& io = ImGui::GetIO();
  io.ConfigWindowsMoveFromTitleBarOnly = true;
  if (io.WantCaptureMouse and Application::Get().isDebugging()) {
    return true;
  } else {
    io.ClearEventsQueue();
  }

  // pitch (rotate around x in radians)
  // yaw (rotate around y in radians)
  // roll (rotate around z in radians)
  entt::entity camera =
    r.view<CCamera, CActiveCamera, CTransform, CUUID>().front();
  ENGINE_ASSERT(camera not_eq entt::null, "No camera found!");
  CCamera& cCamera = r.get<CCamera>(camera);
  CTransform& cTransform = r.get<CTransform>(camera);

  cCamera.rightAngle += e.getX() * cCamera.mouseSensitivity;
  cCamera.upAngle += e.getY() * cCamera.mouseSensitivity;

  cCamera.rightAngle = std::fmod(cCamera.rightAngle, 360.f);
  cCamera.upAngle = std::clamp(cCamera.upAngle, -89.f, 89.f);

  // yaw mouse movement in x-direction
  glm::quat rotY =
    glm::angleAxis(glm::radians(-cCamera.rightAngle), glm::vec3(0, 1, 0));
  // pitch mouse movement in y-direction
  glm::quat rotX =
    glm::angleAxis(glm::radians(cCamera.upAngle), glm::vec3(1, 0, 0));

  cTransform.rotation = rotY * rotX;

  // Normalize the rotation quaternion to prevent drift
  cTransform.rotation = glm::normalize(cTransform.rotation);

  cCamera.calculateView(cTransform.position, cTransform.rotation);

  return true;
}

bool onMouseScrolled(MouseScrolledEvent& e, entt::registry& r) {
  ImGuiIO& io = ImGui::GetIO();
  if (io.WantCaptureMouse and Application::Get().isDebugging()) {
    return true;
  } else {
    io.ClearEventsQueue();
  }

  entt::entity camera =
    r.view<CCamera, CActiveCamera, CTransform, CUUID>().front();
  ENGINE_ASSERT(camera not_eq entt::null, "No camera found!");
  CCamera& cCamera = r.get<CCamera>(camera);
  CTransform& cTransform = r.get<CTransform>(camera);

  cCamera.zoomFactor = std::clamp(cCamera.zoomFactor + float(e.getY()),
                                  cCamera.zoomMin, cCamera.zoomMax);
  cCamera.calculateProjection();

  return true;
}

bool onMouseButtonPressed(MouseButtonPressedEvent& e) {
  ImGuiIO& io = ImGui::GetIO();
  if (io.WantCaptureMouse and Application::Get().isDebugging()) {
    return true;
  } else {
    io.ClearEventsQueue();
  }

  return true;
}

bool onMouseButtonReleased(MouseButtonReleasedEvent& e) {
  ImGuiIO& io = ImGui::GetIO();
  if (io.WantCaptureMouse and Application::Get().isDebugging()) {
    return true;
  } else {
    io.ClearEventsQueue();
  }

  return true;
}

bool onKeyPressed(KeyPressedEvent& e) {
  if (e.getKeyCode() == Key::F3) {
    auto& window = Application::Get().getWindow();
    bool isDebugging = Application::Get().isDebugging();
    if (isDebugging) {
      Application::Get().debug(false);
      window.setCursorMode(CursorMode::Disabled);
      window.restoreCursor(); // imgui does not restore the custom cursor if any
                              // is set
      window.updateCameraPosition(true);
      window.setLastMousePosition(Input::GetMouseX(), Input::GetMouseY());
    } else {
      Application::Get().debug(true);
      window.setCursorMode(CursorMode::Normal);
      window.updateCameraPosition(false);
    }
    return true;
  } else if (e.getKeyCode() == Key::Escape) {
    Application::Get().close();
    return true;
  } else if (e.getKeyCode() == Key::F4) {
    auto& window = Application::Get().getWindow();
    window.toggleWireframe(not window.isWireframe());
    RendererAPI::ToggleWireframe(window.isWireframe());
    return true;
  } else if (e.getKeyCode() == Key::F12) {
    auto& window = Application::Get().getWindow();
    window.setFullscreen(not window.isFullscreen());
    return true;
  }

  ImGuiIO& io = ImGui::GetIO();
  if (io.WantCaptureKeyboard and Application::Get().isDebugging()) {
    return true;
  } else {
    io.ClearEventsQueue();
  }

  if (e.repeating())
    return false;

  auto& window = Application::Get().getWindow();
  // bool control = Input::IsKeyPressed(Key::LeftControl) or
  // Input::IsKeyPressed(Key::RightControl); bool shift =
  // Input::IsKeyPressed(Key::LeftShift) or
  // Input::IsKeyPressed(Key::RightShift);

  switch (e.getKeyCode()) {
  case Key::LeftAlt:
    if (not Application::Get().isDebugging()) {
      window.updateCameraPosition(false);
      window.setCursorMode(CursorMode::Normal);
    }
    break;
  }

  return true;
}

bool onKeyReleased(KeyReleasedEvent& e) {
  ImGuiIO& io = ImGui::GetIO();
  if (io.WantCaptureKeyboard and Application::Get().isDebugging()) {
    return true;
  } else {
    io.ClearEventsQueue();
  }

  auto& window = Application::Get().getWindow();

  switch (e.getKeyCode()) {
  case Key::LeftAlt:
    if (not Application::Get().isDebugging()) {
      window.setLastMousePosition(Input::GetMouseX(), Input::GetMouseY());
      window.updateCameraPosition(true);
      window.setCursorMode(CursorMode::Disabled);
    }
    break;
  }

  return true;
}

bool onKeyTyped(KeyTypedEvent& e) {
  ImGuiIO& io = ImGui::GetIO();
  if (io.WantCaptureKeyboard and Application::Get().isDebugging()) {
    return true;
  } else {
    io.ClearEventsQueue();
  }

  return true;
}

void inputSystem(entt::registry& r, Event& e) {
  EventDispatcher dispatcher(e);

  dispatcher.dispatch<MouseMovedEvent>(BIND_STATIC_EVENT(onMouseMoved, r));
  dispatcher.dispatch<MouseScrolledEvent>(
    BIND_STATIC_EVENT(onMouseScrolled, r));
  dispatcher.dispatch<MouseButtonPressedEvent>(
    BIND_STATIC_EVENT(onMouseButtonPressed));
  dispatcher.dispatch<MouseButtonReleasedEvent>(
    BIND_STATIC_EVENT(onMouseButtonReleased));

  dispatcher.dispatch<KeyPressedEvent>(BIND_STATIC_EVENT(onKeyPressed));
  dispatcher.dispatch<KeyReleasedEvent>(BIND_STATIC_EVENT(onKeyReleased));
  dispatcher.dispatch<KeyTypedEvent>(BIND_STATIC_EVENT(onKeyTyped));
}

}
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
#include "scene/components/input/cActiveInput.h"
#include "scene/components/input/cInput.h"
#include "scene/components/physics/cTransform.h"

namespace potatoengine {

bool onMouseMoved(MouseMovedEvent& e, entt::registry& r) {
  ImGuiIO& io = ImGui::GetIO();
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

  if (cCamera.mode == CCamera::Mode::_2D)
    return true;

  entt::entity movable =
    r.view<CInput, CActiveInput, CTransform, CUUID>().front();
  ENGINE_ASSERT(movable not_eq entt::null, "No movable found!");
  CInput& cInput = r.get<CInput>(movable);

  cCamera.rightAngle += e.getX() * cInput.mouseSensitivity;
  cCamera.upAngle += e.getY() * cInput.mouseSensitivity;

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

  if (cCamera.mode == CCamera::Mode::_2D)
    return true;

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
  auto& app = Application::Get();
  auto& window = app.getWindow();
  bool isDebugging = app.isDebugging();

  if (e.getKeyCode() == Key::F3) {
    if (isDebugging) {
      app.debug(false);
      window.restoreCursor();
      window.toggleCameraPositionUpdate(true);
      window.setLastMousePosition(Input::GetMouseX(), Input::GetMouseY());
    } else {
      app.debug(true);
      window.setCursorMode(CursorMode::Normal, false);
      window.toggleCameraPositionUpdate(false);
    }
    return true;
  } else if (e.getKeyCode() == Key::Escape) {
    if (isDebugging) {
      window.restoreCursor();
    }
    app.close();
    return true;
  } else if (e.getKeyCode() == Key::F4) {
    RendererAPI::ToggleWireframe(not window.isWireframe());
    window.toggleWireframe(not window.isWireframe());
    return true;
  } else if (e.getKeyCode() == Key::F12) {
    window.toggleFullscreen(not window.isFullscreen());
    return true;
  }

  ImGuiIO& io = ImGui::GetIO();
  if (io.WantCaptureKeyboard and isDebugging) {
    return true;
  } else {
    io.ClearEventsQueue();
  }

  if (e.repeating())
    return false;

  // bool control = Input::IsKeyPressed(Key::LeftControl) or
  // Input::IsKeyPressed(Key::RightControl); bool shift =
  // Input::IsKeyPressed(Key::LeftShift) or
  // Input::IsKeyPressed(Key::RightShift);

  switch (e.getKeyCode()) {
  case Key::LeftAlt:
    if (not isDebugging) {
      window.toggleCameraPositionUpdate(false);
      window.setCursorMode(CursorMode::Normal);
    }
    break;
  }

  return true;
}

bool onKeyReleased(KeyReleasedEvent& e) {
  ImGuiIO& io = ImGui::GetIO();
  auto& app = Application::Get();

  if (io.WantCaptureKeyboard and app.isDebugging()) {
    return true;
  } else {
    io.ClearEventsQueue();
  }

  auto& window = app.getWindow();

  switch (e.getKeyCode()) {
  case Key::LeftAlt:
    if (not app.isDebugging()) {
      window.setLastMousePosition(Input::GetMouseX(), Input::GetMouseY());
      window.toggleCameraPositionUpdate(true);
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
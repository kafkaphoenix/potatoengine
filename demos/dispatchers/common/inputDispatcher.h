#pragma once

#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <imgui.h>

#include "engineAPI.h"

namespace demos::dispatchers {

inline bool onMouseMoved(engine::events::MouseMovedEvent& e,
                         entt::registry& registry) {
  ImGuiIO& io = ImGui::GetIO();
  if (io.WantCaptureMouse and engine::Application::Get().isDebugging() or
      engine::Application::Get().isGamePaused()) {
    return true;
  } else {
    io.ClearEventsQueue();
  }

  // pitch (rotate around x in radians)
  // yaw (rotate around y in radians)
  // roll (rotate around z in radians)
  entt::entity camera = registry
                          .view<engine::CCamera, engine::CActiveCamera,
                                engine::CTransform, engine::CUUID>()
                          .front();
  ENGINE_ASSERT(camera not_eq entt::null, "No active camera found!");
  engine::CCamera& cCamera = registry.get<engine::CCamera>(camera);
  engine::CTransform& cTransform = registry.get<engine::CTransform>(camera);

  if (cCamera.mode == engine::CCamera::Mode::_2D)
    return true;

  if (registry.all_of<engine::CActiveInput>(camera)) {
    engine::CInput& cInput = registry.get<engine::CInput>(camera);
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
  } else {
    ENGINE_ASSERT(false, "THIRD PERSON CAMERA NOT IMPLEMENTED");
    entt::entity movable = registry
                             .view<engine::CInput, engine::CActiveInput,
                                   engine::CTransform, engine::CUUID>()
                             .front();
    ENGINE_ASSERT(movable not_eq entt::null, "No movable found!");
    engine::CInput& cInput = registry.get<engine::CInput>(movable);
  }

  return true;
}

inline bool onMouseScrolled(engine::events::MouseScrolledEvent& e,
                            entt::registry& registry) {
  ImGuiIO& io = ImGui::GetIO();
  auto& app = engine::Application::Get();
  if (io.WantCaptureMouse and app.isDebugging() or app.isGamePaused()) {
    return true;
  } else {
    io.ClearEventsQueue();
  }

  entt::entity camera = registry
                          .view<engine::CCamera, engine::CActiveCamera,
                                engine::CTransform, engine::CUUID>()
                          .front();
  ENGINE_ASSERT(camera not_eq entt::null, "No active camera found!");
  engine::CCamera& cCamera = registry.get<engine::CCamera>(camera);

  if (cCamera.mode == engine::CCamera::Mode::_2D)
    return true;

  engine::CTransform& cTransform = registry.get<engine::CTransform>(camera);

  cCamera.zoomFactor = std::clamp(cCamera.zoomFactor + float(e.getY()),
                                  cCamera.zoomMin, cCamera.zoomMax);
  cCamera.calculateProjection();

  return true;
}

inline bool onMouseButtonPressed(engine::events::MouseButtonPressedEvent& e) {
  ImGuiIO& io = ImGui::GetIO();
  if (io.WantCaptureMouse and engine::Application::Get().isDebugging()) {
    return true;
  } else {
    io.ClearEventsQueue();
  }

  return true;
}

inline bool onMouseButtonReleased(engine::events::MouseButtonReleasedEvent& e) {
  ImGuiIO& io = ImGui::GetIO();
  if (io.WantCaptureMouse and engine::Application::Get().isDebugging()) {
    return true;
  } else {
    io.ClearEventsQueue();
  }

  return true;
}

inline bool onKeyPressed(engine::events::KeyPressedEvent& e) {
  auto& app = engine::Application::Get();
  const auto& windows_manager = app.getWindowsManager();
  bool isDebugging = app.isDebugging();
  bool isGamePaused = app.isGamePaused();

  if (e.getKeyCode() == engine::Key::F3) {
    if (isDebugging) {
      app.debug(false);
      windows_manager->restoreCursor();
      windows_manager->toggleCameraPositionUpdate(true);
      windows_manager->setLastMousePosition(engine::Input::GetMouseX(),
                                            engine::Input::GetMouseY());
      if (engine::Application::Get().isRestoreGamePaused()) {
        engine::Application::Get().togglePauseGame(true);
        engine::Application::Get().setRestoreGamePaused(false);
      } else {
        engine::Application::Get().togglePauseGame(false);
      }
    } else {
      app.debug(true);
      if (engine::Application::Get().isGamePaused()) {
        engine::Application::Get().setRestoreGamePaused(true);
      } else {
        engine::Application::Get().togglePauseGame(true);
      }
      glfwSetCursor(windows_manager->getNativeWindow(), nullptr);
      windows_manager->setCursorMode(engine::CursorMode::Normal, false);
      windows_manager->toggleCameraPositionUpdate(false);
    }
    return true;
  } else if (e.getKeyCode() == engine::Key::Escape) {
    // TODO move to layers for flappy bird
    if (isDebugging) {
      windows_manager->restoreCursor();
    }
    app.close();
    return true;
  }

  ImGuiIO& io = ImGui::GetIO();
  if (io.WantCaptureKeyboard and isDebugging) {
    return true;
  } else {
    io.ClearEventsQueue();
  }

  // repeat event if key is held down
  if (e.repeating())
    return false;

  // bool control = Input::IsKeyPressed(Key::LeftControl) or
  // Input::IsKeyPressed(Key::RightControl); bool shift =
  // Input::IsKeyPressed(Key::LeftShift) or
  // Input::IsKeyPressed(Key::RightShift);

  if (e.getKeyCode() == engine::Key::LeftAlt) {
    if (not isDebugging) {
      windows_manager->toggleCameraPositionUpdate(false);
      windows_manager->setCursorMode(engine::CursorMode::Normal, false);
    }
    return true;
  } else if (e.getKeyCode() == engine::Key::P) {
    const auto& settings_manager = app.getSettingsManager();
    if (settings_manager->activeScene not_eq "Flappy Bird") {
      app.togglePauseGame(not isGamePaused);
      return true;
    }
  }

  return false;
}

inline bool onKeyReleased(engine::events::KeyReleasedEvent& e) {
  ImGuiIO& io = ImGui::GetIO();
  auto& app = engine::Application::Get();

  if (io.WantCaptureKeyboard and app.isDebugging()) {
    return true;
  } else {
    io.ClearEventsQueue();
  }

  if (e.getKeyCode() == engine::Key::LeftAlt and not app.isDebugging()) {
    const auto& windows_manager = app.getWindowsManager();
    windows_manager->setLastMousePosition(engine::Input::GetMouseX(),
                                          engine::Input::GetMouseY());
    windows_manager->toggleCameraPositionUpdate(true);
    windows_manager->restoreCursor();
    return true;
  }

  return false;
}

inline bool onKeyTyped(engine::events::KeyTypedEvent& e) {
  ImGuiIO& io = ImGui::GetIO();
  if (io.WantCaptureKeyboard and engine::Application::Get().isDebugging()) {
    return true;
  } else {
    io.ClearEventsQueue();
  }

  return true;
}

inline void inputDispatcher(engine::events::Event& e,
                            entt::registry& registry) {
  engine::events::EventDispatcher dispatcher(e);

  dispatcher.dispatch<engine::events::MouseMovedEvent>(
    BIND_STATIC_EVENT(onMouseMoved, registry));
  dispatcher.dispatch<engine::events::MouseScrolledEvent>(
    BIND_STATIC_EVENT(onMouseScrolled, registry));
  dispatcher.dispatch<engine::events::MouseButtonPressedEvent>(
    BIND_STATIC_EVENT(onMouseButtonPressed));
  dispatcher.dispatch<engine::events::MouseButtonReleasedEvent>(
    BIND_STATIC_EVENT(onMouseButtonReleased));

  dispatcher.dispatch<engine::events::KeyPressedEvent>(
    BIND_STATIC_EVENT(onKeyPressed));
  dispatcher.dispatch<engine::events::KeyReleasedEvent>(
    BIND_STATIC_EVENT(onKeyReleased));
  dispatcher.dispatch<engine::events::KeyTypedEvent>(
    BIND_STATIC_EVENT(onKeyTyped));
}
}
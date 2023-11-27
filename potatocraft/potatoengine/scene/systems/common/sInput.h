#pragma once

#include <entt/entt.hpp>

#include "potatoengine/core/application.h"
#include "potatoengine/core/input.h"
#include "potatoengine/events/keyEvent.h"
#include "potatoengine/events/mouseEvent.h"
#include "potatoengine/events/windowEvent.h"
#include "potatoengine/pch.h"
#include "potatoengine/renderer/rendererAPI.h"
#include "potatoengine/scene/components/camera/cCamera.h"
#include "potatoengine/scene/components/camera/cActiveCamera.h"
#include "potatoengine/scene/components/physics/cTransform.h"
#include "potatoengine/scene/components/common/cUUID.h"

namespace potatoengine {

bool onWindowClosed(entt::registry& r, WindowCloseEvent& e) {
    Application::Get().close();

    return true;
}

bool onWindowRestored(entt::registry& r, WindowRestoredEvent& e) {
    Application::Get().pause(false);

    return true;
}

bool onWindowMinimized(entt::registry& r, WindowMinimizedEvent& e) {
    Application::Get().pause(true);

    return true;
}

bool onWindowMaximized(entt::registry& r, WindowMaximizedEvent& e) {
    return true;
}

bool onWindowFocus(entt::registry& r, WindowFocusEvent& e) {
    return true;
}

bool onWindowMoved(entt::registry& r, WindowMovedEvent& e) {
    return true;
}

bool onWindowResized(entt::registry& r, WindowResizeEvent& e) {
    RendererAPI::SetViewport(0, 0, e.getWidth(), e.getHeight());

    return true;
}

bool onMouseMoved(entt::registry& r, MouseMovedEvent& e) {
    // pitch (rotate around x in radians)
    // yaw (rotate around y in radians)
    // roll (rotate around z in radians)
    entt::entity camera = r.view<CCamera, CActiveCamera, CTransform, CUUID>().front();
    if (camera == entt::null) {
        throw std::runtime_error("No camera found!");
    }
    CCamera& cCamera = r.get<CCamera>(camera);
    CTransform& cTransform = r.get<CTransform>(camera);

    cCamera.rightAngle += e.getX() * cCamera.mouseSensitivity;
    cCamera.upAngle += e.getY() * cCamera.mouseSensitivity;

    cCamera.rightAngle = std::fmod(cCamera.rightAngle, 360.f);
    cCamera.upAngle = std::clamp(cCamera.upAngle, -89.f, 89.f);

    // yaw mouse movement in x-direction
    glm::quat rotY = glm::angleAxis(glm::radians(-cCamera.rightAngle), glm::vec3(0, 1, 0));
    // pitch mouse movement in y-direction
    glm::quat rotX = glm::angleAxis(glm::radians(cCamera.upAngle), glm::vec3(1, 0, 0));

    cTransform.rotation = rotY * rotX;

    // Normalize the rotation quaternion to prevent drift
    cTransform.rotation = glm::normalize(cTransform.rotation);

    cCamera.calculateView(cTransform.position, cTransform.rotation);
    
    return true;
}

bool onMouseScrolled(entt::registry& r, MouseScrolledEvent& e) {
    entt::entity camera = r.view<CCamera, CActiveCamera, CTransform, CUUID>().front();
    if (camera == entt::null) {
        throw std::runtime_error("No camera found!");
    }
    CCamera& cCamera = r.get<CCamera>(camera);
    CTransform& cTransform = r.get<CTransform>(camera);

    cCamera.zoomFactor = std::clamp(cCamera.zoomFactor + float(e.getY()), cCamera.zoomMin, cCamera.zoomMax);
    cCamera.calculateProjection();

    return true;
}

bool onMouseButtonPressed(entt::registry& r, MouseButtonPressedEvent& e) {
    return true;
}

bool onMouseButtonReleased(entt::registry& r, MouseButtonReleasedEvent& e) {
    return true;
}

bool onKeyPressed(entt::registry& r, KeyPressedEvent& e) {
    if (e.repeating())
        return false;

    auto& window = Application::Get().getWindow();
    // bool control = Input::IsKeyPressed(Key::LeftControl) or Input::IsKeyPressed(Key::RightControl);
    // bool shift = Input::IsKeyPressed(Key::LeftShift) or Input::IsKeyPressed(Key::RightShift);

    switch (e.getKeyCode()) {
        case Key::Escape:
            Application::Get().close();
            break;
        case Key::F4:
            window.setWireframe(not window.isWireframe());
            RendererAPI::SetWireframe(window.isWireframe());
            break;
        case Key::F12:
            window.setFullscreen(not window.isFullscreen());
            break;
        case Key::LeftAlt:
            window.updateCameraPosition(false);
            window.setCursorMode(CursorMode::Normal);
            break;
    }

    return true;
}

bool onKeyReleased(entt::registry& r, KeyReleasedEvent& e) {
    auto& window = Application::Get().getWindow();

    switch (e.getKeyCode()) {
        case Key::LeftAlt:
            window.setLastMousePosition(Input::GetMouseX(), Input::GetMouseY());
            window.updateCameraPosition(true);
            window.setCursorMode(CursorMode::Disabled);
            break;
    }

    return true;
}

bool onKeyTyped(entt::registry& r, KeyTypedEvent& e) {
    return true;
}

void inputSystem(entt::registry& r, Event& e) {
    EventDispatcher dispatcher(e);

    dispatcher.dispatch<WindowCloseEvent>(BIND_STATIC_EVENT(onWindowClosed, r));
    dispatcher.dispatch<WindowRestoredEvent>(BIND_STATIC_EVENT(onWindowRestored, r));
    dispatcher.dispatch<WindowMinimizedEvent>(BIND_STATIC_EVENT(onWindowMinimized, r));
    dispatcher.dispatch<WindowMaximizedEvent>(BIND_STATIC_EVENT(onWindowMaximized, r));
    dispatcher.dispatch<WindowResizeEvent>(BIND_STATIC_EVENT(onWindowResized, r));
    dispatcher.dispatch<WindowFocusEvent>(BIND_STATIC_EVENT(onWindowFocus, r));
    dispatcher.dispatch<WindowMovedEvent>(BIND_STATIC_EVENT(onWindowMoved, r));

    dispatcher.dispatch<MouseMovedEvent>(BIND_STATIC_EVENT(onMouseMoved, r));
    dispatcher.dispatch<MouseScrolledEvent>(BIND_STATIC_EVENT(onMouseScrolled, r));
    dispatcher.dispatch<MouseButtonPressedEvent>(BIND_STATIC_EVENT(onMouseButtonPressed, r));
    dispatcher.dispatch<MouseButtonReleasedEvent>(BIND_STATIC_EVENT(onMouseButtonReleased, r));

    dispatcher.dispatch<KeyPressedEvent>(BIND_STATIC_EVENT(onKeyPressed, r));
    dispatcher.dispatch<KeyReleasedEvent>(BIND_STATIC_EVENT(onKeyReleased, r));
    dispatcher.dispatch<KeyTypedEvent>(BIND_STATIC_EVENT(onKeyTyped, r));
}

}
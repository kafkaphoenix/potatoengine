#include "potatoengine/renderer/camera/cameraController.h"

#include "potatoengine/core/input.h"
#include "potatoengine/core/keyCodes.h"
#include "potatoengine/pch.h"

// pitch (rotate around x in radians)
// yaw (rotate around y in radians)
// roll (rotate around z in radians)

namespace potatoengine {

CameraController::CameraController() : m_camera(glm::perspective(glm::radians(m_fov), m_aspectRatio * m_zoomFactor, m_nearClip, m_farClip), m_position, m_rotation) {
}

void CameraController::onUpdate(Time dt) {
    float speed = dt * m_translationSpeed;

    glm::quat qF = m_rotation * glm::quat(0, 0, 0, -1) * glm::conjugate(m_rotation);
    glm::vec3 front = glm::normalize(glm::vec3(qF.x, qF.y, qF.z));
    glm::vec3 right = glm::normalize(glm::cross(front, glm::vec3(0, 1, 0)));
    glm::vec3 up = {0, dt * m_verticalSpeed, 0};

    if (Input::IsKeyPressed(Key::W))
        m_position += front * speed;

    if (Input::IsKeyPressed(Key::S))
        m_position -= front * speed;

    if (Input::IsKeyPressed(Key::A))
        m_position -= right * speed;

    if (Input::IsKeyPressed(Key::D))
        m_position += right * speed;

    if (Input::IsKeyPressed(Key::Space))
        m_position += up;

    if (Input::IsKeyPressed(Key::LeftControl))
        m_position -= up;

    m_camera.setPosition(m_position);
}

void CameraController::onEvent(Event& e) {
    EventDispatcher dispatcher(e);
    dispatcher.dispatch<MouseScrolledEvent>(BIND_EVENT(CameraController::onMouseScrolled));
    dispatcher.dispatch<MouseMovedEvent>(BIND_EVENT(CameraController::onMouseMoved));
    dispatcher.dispatch<WindowResizeEvent>(BIND_EVENT(CameraController::onWindowResized));
}

bool CameraController::onMouseScrolled(MouseScrolledEvent& e) {
    m_zoomFactor = std::clamp(m_zoomFactor + float(e.getY()), m_zoomMin, m_zoomMax);
    m_camera.setProjection(glm::perspective(glm::radians(m_fov * 1.f / m_zoomFactor), m_aspectRatio, m_nearClip, m_farClip));

    return true;
}

bool CameraController::onMouseMoved(MouseMovedEvent& e) {
    m_rightAngle += e.getX() * m_mouseSensitivity;
    m_upAngle += e.getY() * m_mouseSensitivity;

    m_rightAngle = std::fmod(m_rightAngle, 360.f);
    m_upAngle = std::clamp(m_upAngle, -89.f, 89.f);

    // yaw mouse movement in x-direction
    glm::quat rotY = glm::angleAxis(glm::radians(-m_rightAngle), glm::vec3(0, 1, 0));
    // pitch mouse movement in y-direction
    glm::quat rotX = glm::angleAxis(glm::radians(m_upAngle), glm::vec3(1, 0, 0));

    m_rotation = rotY * rotX;

    // Normalize the rotation quaternion to prevent drift
    m_rotation = glm::normalize(m_rotation);

    m_camera.setRotation(m_rotation);

    return true;
}

bool CameraController::onWindowResized(WindowResizeEvent& e) {
    m_aspectRatio = (float)e.getWidth() / (float)e.getHeight();
    m_camera.setProjection(glm::perspective(glm::radians(m_fov), m_aspectRatio * m_zoomFactor, 1.f, 3000.f));

    return true;
}

}
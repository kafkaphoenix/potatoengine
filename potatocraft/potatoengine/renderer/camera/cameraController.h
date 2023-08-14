#pragma once

#include "potatoengine/core/time.h"
#include "potatoengine/events/applicationEvent.h"
#include "potatoengine/events/mouseEvent.h"
#include "potatoengine/renderer/camera/camera.h"

namespace potatoengine {

class CameraController {
   public:
    CameraController();

    void onUpdate(Time ts);
    void onEvent(Event& e);

    float getAspectRatio() const noexcept { return m_aspectRatio; }
    void setAspectRatio(float aspectRatio) noexcept { m_aspectRatio = aspectRatio; }
    float getFov() const noexcept { return m_fov; }
    void setFov(float fov) noexcept { m_fov = fov; }
    float getZoomFactor() const noexcept { return m_zoomFactor; }
    void setZoomFactor(float level) noexcept { m_zoomFactor = level; }
    float getMouseSensitivity() const noexcept { return m_mouseSensitivity; }
    void setMouseSensitivity(float sensitivity) noexcept { m_mouseSensitivity = sensitivity; }

    float getTranslationSpeed() const noexcept { return m_translationSpeed; }
    void setTranslationSpeed(float speed) noexcept { m_translationSpeed = speed; }
    float getVerticalSpeed() const noexcept { return m_verticalSpeed; }
    void setVerticalSpeed(float speed) noexcept { m_verticalSpeed = speed; }
    float getRotationSpeed() const noexcept { return m_rotationSpeed; }
    void setRotationSpeed(float speed) noexcept { m_rotationSpeed = speed; }

    Camera& getCamera() noexcept { return m_camera; }
    const Camera& getCamera() const noexcept { return m_camera; }

   private:
    bool onMouseScrolled(MouseScrolledEvent& e);
    bool onMouseMoved(MouseMovedEvent& e);
    bool onWindowResized(WindowResizeEvent& e);

    float m_aspectRatio = 3840.f / 2160.f;
    float m_fov = 90.f;
    float m_zoomFactor = 1.f;
    float m_zoomMin = 1.f;
    float m_zoomMax = 4.f;
    float m_mouseSensitivity = 0.1f;
    float m_nearClip = 1.f;
    float m_farClip = 3000.f;

    glm::vec3 m_position = glm::vec3(0.f, 0.f, 5.f);
    glm::quat m_rotation = glm::identity<glm::quat>();
    float m_rightAngle{};
    float m_upAngle{};

    float m_translationSpeed = 10.f;
    float m_verticalSpeed = 45.f;
    float m_rotationSpeed = 180.f;

    Camera m_camera;
};

}
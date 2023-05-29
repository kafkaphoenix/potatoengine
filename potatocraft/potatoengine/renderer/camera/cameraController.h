#pragma once

#include "potatoengine/core/time.h"
#include "potatoengine/events/applicationEvent.h"
#include "potatoengine/events/mouseEvent.h"
#include "potatoengine/renderer/camera/camera.h"

namespace potatoengine {

class CameraController {
   public:
    CameraController();

    void onUpdate(Time dt);
    void onEvent(Event& e);

    float getAspectRatio() const { return m_aspectRatio; }
    void setAspectRatio(float aspectRatio) { m_aspectRatio = aspectRatio; }
    float getFov() const { return m_fov; }
    void setFov(float fov) { m_fov = fov; }
    float getZoomLevel() const { return m_zoomLevel; }
    void setZoomLevel(float level) { m_zoomLevel = level; }
    float getMouseSensitivity() const { return m_mouseSensitivity; }
    void setMouseSensitivity(float sensitivity) { m_mouseSensitivity = sensitivity; }

    float getTranslationSpeed() const { return m_translationSpeed; }
    void setTranslationSpeed(float speed) { m_translationSpeed = speed; }
    float getVerticalSpeed() const { return m_verticalSpeed; }
    void setVerticalSpeed(float speed) { m_verticalSpeed = speed; }
    float getRotationSpeed() const { return m_rotationSpeed; }
    void setRotationSpeed(float speed) { m_rotationSpeed = speed; }

    Camera& getCamera() { return m_camera; }
    const Camera& getCamera() const { return m_camera; }

   private:
    bool onMouseScrolled(MouseScrolledEvent& e);
    bool onMouseMoved(MouseMovedEvent& e);
    bool onWindowResized(WindowResizeEvent& e);

    float m_aspectRatio = 3840.f / 2160.f;
    float m_fov = 90.f;
    float m_zoomLevel = 1.f;
    float m_zoomMin = 1.f;
    float m_zoomMax = 4.f;
    float m_mouseSensitivity = 0.1f;
    float m_nearClip = 1.f;
    float m_farClip = 3000.f;

    glm::vec3 m_position = {0.f, 0.f, 0.f};
    glm::quat m_orientation = {1.f, 0.f, 0.f, 0.f};
    float m_rightAngle = 0.f;
    float m_upAngle = 0.f;

    float m_translationSpeed = 10.f;
    float m_verticalSpeed = 45.f;
    float m_rotationSpeed = 180.f;

    Camera m_camera;
};

}
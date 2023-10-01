#include "potatoengine/renderer/camera/camera.h"

#include "potatoengine/pch.h"

namespace potatoengine {
Camera::Camera(const glm::mat4& projection, const glm::vec3& position, const glm::quat& rotation) : m_projection(projection), m_position(position), m_rotation(rotation) {
    recalculateView();  // TODO move to on attach on component?
}

void Camera::recalculateView() noexcept {
    glm::mat4 rotated = glm::mat4_cast(glm::conjugate(m_rotation));
    glm::mat4 translated = glm::translate(rotated, -m_position);
    m_view = std::move(translated);
}

void Camera::setProjection(glm::mat4&& p) noexcept {
    m_projection = std::move(p);
}

void Camera::setRotation(glm::quat&& r) noexcept {
    m_rotation = std::move(r);
    recalculateView();
}

void Camera::setPosition(glm::vec3&& p) noexcept {
    m_position = std::move(p);
    recalculateView();
}

}
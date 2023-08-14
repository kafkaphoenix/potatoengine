#include "potatoengine/renderer/camera/camera.h"

#include "potatoengine/pch.h"

namespace potatoengine {
Camera::Camera(const glm::mat4& proj, const glm::vec3& pos, const glm::quat& rot) : m_projection(proj), m_position(pos), m_rotation(rot) {
    recalculateView();  // TODO move to on attach on component?
}

void Camera::recalculateView() noexcept {
    glm::mat4 rotated = glm::mat4_cast(glm::conjugate(m_rotation));
    glm::mat4 translated = glm::translate(rotated, -m_position);
    m_view = translated;
    m_viewProjection = m_projection * m_view;
}

void Camera::setProjection(const glm::mat4& proj) noexcept {
    m_projection = proj;
    m_viewProjection = proj * m_view;
}

void Camera::setRotation(const glm::quat& rot) noexcept {
    m_rotation = rot;
    recalculateView();
}

void Camera::setPosition(const glm::vec3& pos) noexcept {
    m_position = pos;
    recalculateView();
}

}
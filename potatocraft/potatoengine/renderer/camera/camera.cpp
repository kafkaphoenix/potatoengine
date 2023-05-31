#include "potatoengine/renderer/camera/camera.h"

#include "potatoengine/pch.h"

namespace potatoengine {
Camera::Camera(const glm::mat4& projection, const glm::vec3& position, const glm::quat& rotation) : m_projection(projection),
                                                                                                       m_position(position),
                                                                                                       m_rotation(rotation) {
    recalculateView();
}

void Camera::recalculateView() {
    glm::mat4 rotated = glm::mat4_cast(glm::conjugate(m_rotation));
    glm::mat4 translated = glm::translate(rotated, -m_position);
    m_view = translated;
    m_viewProjection = m_projection * m_view;
}

}
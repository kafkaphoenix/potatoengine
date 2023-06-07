#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace potatoengine {

class Camera {
   public:
    Camera() = default;
    Camera(const glm::mat4& projection, const glm::vec3& position, const glm::quat& rotation);
    void setProjection(const glm::mat4& projection) {
        m_projection = projection;
        m_viewProjection = m_projection * m_view;
    }

    const glm::mat4& getView() const { return m_view; }
    const glm::mat4& getProjection() const { return m_projection; }
    const glm::mat4& getViewProjection() const { return m_viewProjection; }

    const glm::vec3& getPosition() const { return m_position; }
    void setPosition(const glm::vec3& position) {
        m_position = position;
        recalculateView();
    }
    const glm::quat& getRotation() const { return m_rotation; }
    void setRotation(const glm::quat& rotation) {
        m_rotation = rotation;
        recalculateView();
    }

   private:
    void recalculateView();

    glm::mat4 m_view;
    glm::mat4 m_projection;
    glm::mat4 m_viewProjection;

    glm::vec3 m_position;
    glm::quat m_rotation;
};

}
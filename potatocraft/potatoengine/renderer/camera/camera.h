#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace potatoengine {

class Camera {
   public:
    Camera() = default;
    Camera(const glm::mat4& proj, const glm::vec3& pos, const glm::quat& rot);

    const glm::mat4& getView() const noexcept { return m_view; }
    const glm::mat4& getProjection() const noexcept { return m_projection; }
    const glm::mat4& getViewProjection() const noexcept { return m_viewProjection; }
    const glm::vec3& getPosition() const noexcept { return m_position; }
    const glm::quat& getRotation() const noexcept { return m_rotation; }
    void setProjection(const glm::mat4& proj) noexcept;
    void setPosition(const glm::vec3& pos) noexcept;
    void setRotation(const glm::quat& rot) noexcept;

   private:
    void recalculateView() noexcept;

    glm::mat4 m_view = glm::mat4(1.0f);
    glm::mat4 m_projection = glm::mat4(1.0f);
    glm::mat4 m_viewProjection = glm::mat4(1.0f);

    glm::vec3 m_position = glm::vec3(0.0f);
    glm::quat m_rotation = glm::identity<glm::quat>();
};

}
#pragma once

#define GLM_FORCE_CTOR_INIT
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace potatoengine {

class Camera {
   public:
    Camera() = default;
    Camera(const glm::mat4& projection, const glm::vec3& position, const glm::quat& rotation);

    const glm::mat4& getView() const noexcept { return m_view; }
    const glm::mat4& getProjection() const noexcept { return m_projection; }
    const glm::vec3& getPosition() const noexcept { return m_position; }
    const glm::quat& getRotation() const noexcept { return m_rotation; }
    void setProjection(glm::mat4&& p) noexcept;
    void setPosition(glm::vec3&& p) noexcept;
    void setRotation(glm::quat&& r) noexcept;

   private:
    void recalculateView() noexcept;

    glm::mat4 m_view{};
    glm::mat4 m_projection{};
    glm::vec3 m_position{};
    glm::quat m_rotation{};
};

}
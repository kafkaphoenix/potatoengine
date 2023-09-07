#pragma once

#include <glm/glm.hpp>

#include "potatoengine/assets/shader.h"
#include "potatoengine/pch.h"

namespace potatoengine {

class ShaderProgram {
   public:
    ShaderProgram(const std::string &name);
    ~ShaderProgram();

    void attach(const Shader &s) const noexcept;
    void detach(const Shader &s) const noexcept;
    void link() const;
    void use() const noexcept;
    void unuse() const noexcept;
    void setInt(std::string_view name, int value) const noexcept;
    void setFloat(std::string_view name, float value) const noexcept;
    void setVec2(std::string_view name, const glm::vec2 &vec) const noexcept;
    void setVec3(std::string_view name, const glm::vec3 &vec) const noexcept;
    void setVec4(std::string_view name, const glm::vec4 &vec) const noexcept;
    void setMat4(std::string_view name, const glm::mat4 &mat) const noexcept;
    std::string_view getName() const noexcept { return m_name; }
    uint32_t getID() const noexcept { return m_id; }
    operator GLuint() const;

    static std::unique_ptr<ShaderProgram> Create(const std::string &name) noexcept;

   private:
    uint32_t m_id{};
    std::string m_name{};
};

}
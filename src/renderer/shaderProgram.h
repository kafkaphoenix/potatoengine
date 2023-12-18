#pragma once

#include <glm/glm.hpp>

#include "assets/shader.h"
#include "pch.h"
#include "utils/numericComparator.h"

namespace potatoengine {

struct ActiveUniform {
    uint32_t type;
    std::string name;
};

class ShaderProgram {
  public:
    ShaderProgram(std::string&& name);
    ~ShaderProgram();

    void attach(const Shader& s);
    void detach(const Shader& s);
    void link();
    void use();
    void unuse();
    void setInt(std::string_view name, int value);
    void setFloat(std::string_view name, float value);
    void setVec2(std::string_view name, const glm::vec2& vec);
    void setVec3(std::string_view name, const glm::vec3& vec);
    void setVec4(std::string_view name, const glm::vec4& vec);
    void setMat4(std::string_view name, const glm::mat4& mat);
    void resetActiveUniforms();
    void printActiveUniforms();
    const std::map<std::string, std::string, NumericComparator>& getInfo();

    std::string_view getName() const noexcept { return m_name; }
    uint32_t getID() const noexcept { return m_id; }
    operator GLuint() const;

    static std::unique_ptr<ShaderProgram> Create(std::string&& name);

  private:
    uint32_t m_id{};
    std::string m_name{};
    std::vector<ActiveUniform> m_activeUniforms{};
    std::map<std::string, std::string, NumericComparator> m_info{};

    std::vector<ActiveUniform> getActiveUniforms();
};

}
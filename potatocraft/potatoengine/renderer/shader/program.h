#pragma once

#include <glm/glm.hpp>

#include "potatoengine/pch.h"
#include "potatoengine/renderer/shader/shader.h"

namespace potatoengine {

class Program {
   public:
    Program(const std::string &name);
    ~Program();

    operator GLuint() const;
    void attach(const Shader &shader);
    void detach(const Shader &shader);
    void link();
    void use();
    void unuse();
    void setInt(const std::string &name, int value);
    void setFloat(const std::string &name, float value);
    void setVec2(const std::string &name, const glm::vec2 &vec);
    void setVec3(const std::string &name, const glm::vec3 &vec);
    void setVec4(const std::string &name, const glm::vec4 &vec);
    void setMat4(const std::string &name, const glm::mat4 &mat);
    std::string_view getName() const noexcept { return m_name; }

    static std::shared_ptr<Program> Create(const std::string &name);

   private:
    uint32_t m_program;
    std::string m_name{};
};

}
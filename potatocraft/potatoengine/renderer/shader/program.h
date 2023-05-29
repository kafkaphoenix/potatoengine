#pragma once

#include <glm/glm.hpp>

#include "potatoengine/pch.h"
#include "potatoengine/renderer/shader/shader.h"

namespace potatoengine {

class Program {
   public:
    Program(const std::string &name);
    ~Program();

    operator GLuint();
    void attach(Shader &shader);
    void detach(Shader &shader);
    void link(bool &status);
    void use();
    void setInt(const std::string &name, int value);
    void setFloat(const std::string &name, float value);
    void setVec2(const std::string &name, const glm::vec2 &vec);
    void setVec3(const std::string &name, const glm::vec3 &vec);
    void setVec4(const std::string &name, const glm::vec4 &vec);
    void setMat4(const std::string &name, const glm::mat4 &mat);
    const std::string &getName() const noexcept;
    static std::shared_ptr<Program> Create(const std::string &name);

   private:
    uint32_t m_program;
    std::string m_name;
};

}
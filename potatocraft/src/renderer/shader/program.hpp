#pragma once

#include <src/renderer/shader/shader.hpp>

#include <glm/glm.hpp>

namespace potatocraft {

    class Program
    {
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

    private:
        GLuint m_program;
        std::string m_name;
    };

}
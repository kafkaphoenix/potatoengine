#ifndef PROGRAM_H
#define PROGRAM_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <src/shader/shader.hpp>

class Program
{
private:
    GLuint m_program;
    std::string m_name;

public:
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

    Program(const std::string &name);
    ~Program();
};

#endif PROGRAM_H
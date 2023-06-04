#include "potatoengine/renderer/shader/program.h"

#include <glm/gtc/type_ptr.hpp>

namespace potatoengine {
Program::Program(const std::string &name) : m_program(glCreateProgram()), m_name(name) {
}

Program::~Program() {
    glDeleteProgram(m_program);
}

Program::operator GLuint() const {
    return m_program;
}

void Program::attach(const Shader &shader) {
    glAttachShader(m_program, shader);
}

void Program::detach(const Shader &shader) {
    glDetachShader(m_program, shader);
}

void Program::link() {
    glLinkProgram(m_program);

    int status = GL_FALSE;
    glGetProgramiv(m_program, GL_LINK_STATUS, &status);
    if (status not_eq GL_TRUE) {
        int infoLogLength = 0;
        glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &infoLogLength);
        if (infoLogLength == 0) {
            throw std::runtime_error("Could not link shader program " + m_name);
        }
        std::vector<GLchar> shaderProgramInfoLog(infoLogLength);
        glGetProgramInfoLog(m_program, infoLogLength, &infoLogLength, shaderProgramInfoLog.data());
        throw std::runtime_error("Could not link shader program " + m_name + ": " + shaderProgramInfoLog.data());
    }
}

void Program::use() {
    glUseProgram(m_program);
}

void Program::setInt(const std::string &name, int value) {
    glUniform1i(glGetUniformLocation(m_program, name.c_str()), value);
}

void Program::setFloat(const std::string &name, float value) {
    glUniform1f(glGetUniformLocation(m_program, name.c_str()), value);
}

void Program::setVec2(const std::string &name, const glm::vec2 &vec) {
    glUniform2f(glGetUniformLocation(m_program, name.c_str()), vec.x, vec.y);
}

void Program::setVec3(const std::string &name, const glm::vec3 &vec) {
    glUniform3f(glGetUniformLocation(m_program, name.c_str()), vec.x, vec.y, vec.z);
}

void Program::setVec4(const std::string &name, const glm::vec4 &vec) {
    glUniform4f(glGetUniformLocation(m_program, name.c_str()), vec.x, vec.y, vec.z, vec.w);
}

void Program::setMat4(const std::string &name, const glm::mat4 &mat) {
    glUniformMatrix4fv(glGetUniformLocation(m_program, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}

const std::string &Program::getName() const noexcept {
    return m_name;
}

std::shared_ptr<Program> Program::Create(const std::string &name) {
    return std::make_shared<Program>(name);
}

}
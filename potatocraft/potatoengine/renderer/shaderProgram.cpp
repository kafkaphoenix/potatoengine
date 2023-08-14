#include "potatoengine/renderer/shaderProgram.h"

#include <glm/gtc/type_ptr.hpp>

namespace potatoengine {
ShaderProgram::ShaderProgram(const std::string &name) : m_id(glCreateProgram()), m_name(name) {
}

ShaderProgram::~ShaderProgram() {
    glDeleteProgram(m_id);
}

ShaderProgram::operator GLuint() const {
    return static_cast<GLuint>(m_id);
}

void ShaderProgram::attach(const Shader &s) const noexcept {
    glAttachShader(m_id, s);
}

void ShaderProgram::detach(const Shader &s) const noexcept {
    glDetachShader(m_id, s);
}

void ShaderProgram::link() const {
    glLinkProgram(m_id);

    int status = GL_FALSE;
    glGetProgramiv(m_id, GL_LINK_STATUS, &status);
    if (status not_eq GL_TRUE) [[unlikely]] {
        int infoLogLength = 0;
        glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &infoLogLength);
        if (infoLogLength == 0) {
            throw std::runtime_error("Could not link shader program " + m_name);
        }
        std::vector<GLchar> shaderProgramInfoLog(infoLogLength);
        glGetProgramInfoLog(m_id, infoLogLength, &infoLogLength, shaderProgramInfoLog.data());
        throw std::runtime_error("Could not link shader program " + m_name + ": " + shaderProgramInfoLog.data());
    }
}

void ShaderProgram::use() const noexcept {
    glUseProgram(m_id);
}

void ShaderProgram::unuse() const noexcept {
    glUseProgram(0);
}

void ShaderProgram::setInt(std::string_view name, int value) const noexcept {
    glUniform1i(glGetUniformLocation(m_id, name.data()), value);
}

void ShaderProgram::setFloat(std::string_view name, float value) const noexcept {
    glUniform1f(glGetUniformLocation(m_id, name.data()), value);
}

void ShaderProgram::setVec2(std::string_view name, const glm::vec2 &vec) const noexcept {
    glUniform2f(glGetUniformLocation(m_id, name.data()), vec.x, vec.y);
}

void ShaderProgram::setVec3(std::string_view name, const glm::vec3 &vec) const noexcept {
    glUniform3f(glGetUniformLocation(m_id, name.data()), vec.x, vec.y, vec.z);
}

void ShaderProgram::setVec4(std::string_view name, const glm::vec4 &vec) const noexcept {
    glUniform4f(glGetUniformLocation(m_id, name.data()), vec.x, vec.y, vec.z, vec.w);
}

void ShaderProgram::setMat4(std::string_view name, const glm::mat4 &mat) const noexcept {
    glUniformMatrix4fv(glGetUniformLocation(m_id, name.data()), 1, GL_FALSE, glm::value_ptr(mat));
}

std::unique_ptr<ShaderProgram> ShaderProgram::Create(const std::string &name) noexcept {
    return std::make_unique<ShaderProgram>(name);
}

}
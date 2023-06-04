#include "potatoengine/renderer/shader/shader.h"

#include "potatoengine/pch.h"

namespace potatoengine {
Shader::Shader(const std::string& filepath) : m_filepath(filepath) {
    std::ifstream file(filepath);
    if (not file.is_open()) {
        file.close();
        throw std::runtime_error("Failed to load file: " + filepath);
    }
    std::string data(std::istreambuf_iterator<char>(file), {});
    file.close();

    std::string extension = filepath.substr(filepath.find_last_of(".") + 1);
    m_type = (extension == "vert") ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER;
    m_id = glCreateShader(m_type);

    const GLchar* source = data.c_str();
    glShaderSource(m_id, 1, &source, 0);
    glCompileShader(m_id);

    int32_t status = GL_FALSE;
    glGetShaderiv(m_id, GL_COMPILE_STATUS, &status);
    if (status not_eq GL_TRUE) {
        glDeleteShader(m_id);
        GLint infoLogLength = 0;
        glGetShaderiv(m_id, GL_INFO_LOG_LENGTH, &infoLogLength);
        if (infoLogLength == 0) {
            throw std::runtime_error("Shader compilation failed");
        }
        std::vector<GLchar> shaderInfoLog(infoLogLength);
        glGetShaderInfoLog(m_id, infoLogLength, &infoLogLength, shaderInfoLog.data());
        throw std::runtime_error("Shader compilation failed: " + std::string(shaderInfoLog.data()));
    }
}

Shader::~Shader() {
    glDeleteShader(m_id);
}
}
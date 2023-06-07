#include "potatoengine/renderer/shader/shader.h"

#include "potatoengine/pch.h"

namespace potatoengine {
Shader::Shader(const std::filesystem::path& fp) : m_filepath(fp.string()) {
    std::ifstream f(fp);
    if (not f.is_open()) [[unlikely]] {
        f.close();
        throw std::runtime_error("Failed to load shader: " + m_filepath);
    }
    std::string data(std::istreambuf_iterator<char>(f), {});
    f.close();

    m_type = fp.extension() == ".vert" ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER;
    m_id = glCreateShader(m_type);

    const GLchar* source = data.c_str();
    glShaderSource(m_id, 1, &source, 0);
    glCompileShader(m_id);

    int32_t status = GL_FALSE;
    glGetShaderiv(m_id, GL_COMPILE_STATUS, &status);
    if (status not_eq GL_TRUE) [[unlikely]] {
        glDeleteShader(m_id);
        GLint infoLogLength = 0;
        glGetShaderiv(m_id, GL_INFO_LOG_LENGTH, &infoLogLength);
        if (infoLogLength == 0) {
            throw std::runtime_error("Shader compilation failed!");
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
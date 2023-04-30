#include "src/pch.h"
#include "src/renderer/shader/shader.h"

namespace potatoengine
{
    Shader::Shader() : m_id(-1) {}

    Shader::~Shader()
    {
        if (*this)
        glDeleteShader(m_id);
    }

    bool readfile(const char *filenamePath, std::string &contents)
    {
        std::ifstream f(filenamePath);
        if (not f.good())
        {
        fprintf(stdout, "Failed to load file from path %s\n", filenamePath);
        return false;
        }
        char c;
        while ((c = f.get()), f.good())
        {
        contents += c;
        }
        f.close();
        return true;
    }

    void Shader::create(GLenum type)
    {
        m_type = type;
        m_id = glCreateShader(type);
    }

    bool Shader::load_src(GLenum type, const std::string &src)
    {
        this->create(type);
        this->set_src(src);
        this->compile();
        int32_t status = GL_FALSE;
        glGetShaderiv(m_id, GL_COMPILE_STATUS, &status);
        if (status not_eq GL_TRUE)
        {
        int32_t maxLength = 0;
        glGetShaderiv(m_id, GL_INFO_LOG_LENGTH, &maxLength);
        fprintf(stdout, "Shader compilation failed\n");
        if (maxLength > 0)
        {
            std::vector<GLchar> shaderInfoLog;
            shaderInfoLog.reserve(maxLength);
            glGetShaderInfoLog(m_id, maxLength, &maxLength, &shaderInfoLog[0]);
            fprintf(stdout, "\tError info: %s\n", shaderInfoLog.data());
        }
        return false;
        }
        return true;
    }

    void Shader::load_file(GLenum type, const std::string &file, bool &status)
    {
        std::string src;
        if (readfile(file.c_str(), src))
        {
        status = this->load_src(type, src);
        }
    }

    void Shader::set_src(const std::string &src)
    {
        const GLchar *src_cstr = (const GLchar *)src.c_str();
        glShaderSource(m_id, 1, &src_cstr, 0);
    }

    void Shader::set_file(const std::string &file)
    {
        std::string src;
        readfile(file.c_str(), src);
        this->set_src(src);
    }

    void Shader::compile()
    {
        glCompileShader(m_id);
    }

    Shader::operator GLuint()
    {
        return m_id;
    }

    Shader::operator bool()
    {
        return m_id not_eq static_cast<uint32_t>(-1);
    }

}
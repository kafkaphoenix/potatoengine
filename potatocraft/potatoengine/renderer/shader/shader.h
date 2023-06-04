#pragma once

#include <glad/glad.h>

namespace potatoengine {

class Shader {
   public:
    Shader(const std::string& filepath);
    ~Shader();

    uint32_t getID() const { return m_id; }
    const std::string& getFilepath() const { return m_filepath; }

    bool operator==(const Shader& other) const {
        return m_id == other.getID() && m_type == other.m_type;
    }
    operator GLuint() const { return m_id; }

   private:
    std::string m_filepath;
    uint32_t m_id;
    GLenum m_type;
};

}
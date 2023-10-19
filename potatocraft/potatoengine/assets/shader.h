#pragma once

#include <glad/glad.h>

namespace potatoengine {

class Shader {
   public:
    Shader(std::filesystem::path&& fp);
    ~Shader();
    Shader& operator=(const Shader&) = delete;

    uint32_t getID() const noexcept { return m_id; }
    std::string_view getFilepath() const noexcept { return m_filepath; }

    bool operator==(const Shader& other) const noexcept {
        return m_id == other.getID() and m_type == other.m_type;
    }
    operator GLuint() const noexcept { return m_id; }

   private:
    std::string m_filepath{};
    uint32_t m_id{};
    GLenum m_type{};
};

}
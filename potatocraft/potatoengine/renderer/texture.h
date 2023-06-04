#pragma once

#include <glad/glad.h>

namespace potatoengine {

class Texture {
   public:
    Texture(const std::string& path);
    ~Texture();

    uint32_t getWidth() const { return m_width; }
    uint32_t getHeight() const { return m_height; }
    uint32_t getID() const { return m_id; }
    const std::string& getFilepath() const { return m_filepath; }

    void bind(uint32_t slot) const;

    bool operator==(const Texture& other) const {
        return m_id == other.getID() && m_width == other.getWidth() && m_height == other.getHeight();
    }

   private:
    std::string m_filepath;
    uint32_t m_width, m_height;
    uint32_t m_id;
    GLenum m_GLFormat{}, m_format{};
};

}
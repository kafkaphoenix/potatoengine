#pragma once

#include <glad/glad.h>

namespace potatoengine {
class Texture {
   public:
    Texture(const std::filesystem::path& fp);
    ~Texture();

    uint32_t getWidth() const { return m_width; }
    uint32_t getHeight() const { return m_height; }
    uint32_t getID() const { return m_id; }
    std::string_view getFilepath() const noexcept { return m_filepath; }
    void bindSlot(uint32_t slot) const;

    bool operator==(const Texture& other) const {
        return m_id == other.getID() and m_width == other.getWidth() and m_height == other.getHeight();
    }

   private:
    std::string m_filepath{};
    uint32_t m_width, m_height;
    uint32_t m_id;
    GLenum m_GLFormat, m_format;
};
}
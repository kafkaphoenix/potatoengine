#pragma once

#include <glad/glad.h>

namespace potatoengine {
class Texture {
   public:
    Texture() = default;
    Texture(const std::filesystem::path& fp, const std::optional<std::string>& type = std::nullopt, std::optional<bool> gammaCorrection = std::nullopt);
    ~Texture();

    uint32_t getWidth() const noexcept { return m_width; }
    uint32_t getHeight() const noexcept { return m_height; }
    uint32_t getID() const noexcept { return m_id; }
    std::string_view getFilepath() const noexcept { return m_filepath; }
    std::string_view getType() const noexcept { return m_type; }
    void bindSlot(uint32_t slot) noexcept;
    void rebindSlot() const noexcept;

    bool operator==(const Texture& other) const noexcept {
        return m_id == other.getID() and m_width == other.getWidth() and m_height == other.getHeight() and m_type == other.getType();
    }

   private:
    std::string m_filepath{};
    std::string m_type{};
    uint32_t m_width{}, m_height{};
    uint32_t m_id{};
    GLenum m_GLFormat{}, m_format{};
    int m_slot{};

    void loadTexture();
};
}
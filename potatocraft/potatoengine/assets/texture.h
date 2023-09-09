#pragma once

#include <glad/glad.h>

namespace potatoengine {
class Texture {
   public:
    Texture() = default;
    Texture(const std::filesystem::path& fp, const std::optional<std::string>& type = std::nullopt, std::optional<bool> flipVertically = std::nullopt, std::optional<int> mipmap_level = std::nullopt, std::optional<bool> gammaCorrection = std::nullopt);
    ~Texture();

    uint32_t getWidth() const noexcept { return m_width; }
    uint32_t getHeight() const noexcept { return m_height; }
    uint32_t getID() const noexcept { return m_id; }
    std::string_view getFilepath() const noexcept { return (m_filepaths.size() == 1) ? m_filepaths[0] : m_directory; }
    std::string_view getType() const noexcept { return m_type; }
    void bindSlot(uint32_t slot);
    void rebindSlot();
    void unbindSlot();

    bool operator==(const Texture& other) const noexcept {
        return m_id == other.getID() and m_width == other.getWidth() and m_height == other.getHeight() and m_type == other.getType();
    }

    static constexpr bool DONT_FLIP_VERTICALLY = false;
    static constexpr bool FLIP_VERTICALLY = true;
    static constexpr bool GAMMA_CORRECTION = true;
    static constexpr bool NO_GAMMA_CORRECTION = false;

   private:
    std::vector<std::string> m_filepaths{};
    std::string m_directory{};
    std::string m_type{};
    uint32_t m_width{}, m_height{};
    uint32_t m_id{};
    GLenum m_GLFormat{}, m_format{};
    int m_slot{};
    bool m_isCubemap{};
    bool m_flipVertically{true};
    int m_mipmap_level{};
    bool m_gammaCorrection{};

    void loadTexture();
};
}
#include "potatoengine/assets/texture.h"

#include <stb_image.h>

#include "potatoengine/pch.h"

namespace potatoengine {

Texture::Texture(uint32_t width, uint32_t height, GLenum glFormat, std::optional<bool> wrap) : m_width(width), m_height(height), m_glFormat(glFormat) {
    glCreateTextures(GL_TEXTURE_2D, 1, &m_id);
    glTextureStorage2D(m_id, 1, m_glFormat, m_width, m_height);
    glTextureParameteri(m_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    if (wrap.value_or(false)) {
        glTextureParameteri(m_id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // prevents edge bleeding fbo
        glTextureParameteri(m_id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }
    m_mipmapLevel = 1;
    m_flipVertically = false;
    m_filepaths.emplace_back("fbo texture");
}

Texture::Texture(std::filesystem::path&& fp, std::optional<std::string>&& type, std::optional<bool> flipVertically, std::optional<uint32_t> mipmap_level, std::optional<bool> gammaCorrection)
    : m_directory(std::filesystem::is_directory(fp) ? std::move(fp.string()) : ""),
      m_isCubemap(std::filesystem::is_directory(fp)),
      m_type(std::move(type.value_or(""))),
      m_flipVertically(flipVertically.value_or(true)),
      m_mipmapLevel(mipmap_level.value_or(4)),
      m_gammaCorrection(gammaCorrection.value_or(false)) {
    if (m_isCubemap) {
        std::string fileExt = std::filesystem::exists(fp / "front.jpg") ? ".jpg" : ".png";
        m_filepaths.emplace_back(std::move((fp / ("front" + fileExt)).string()));  // it needs to be added in this order
        m_filepaths.emplace_back(std::move((fp / ("back" + fileExt)).string()));
        m_filepaths.emplace_back(std::move((fp / ("top" + fileExt)).string()));
        m_filepaths.emplace_back(std::move((fp / ("bottom" + fileExt)).string()));
        m_filepaths.emplace_back(std::move((fp / ("right" + fileExt)).string()));
        m_filepaths.emplace_back(std::move((fp / ("left" + fileExt)).string()));
    } else {
        m_filepaths.emplace_back(std::move(fp.string()));
    }

    loadTexture();
}

void Texture::loadTexture() {
    int width, height, channels;
    stbi_set_flip_vertically_on_load(m_flipVertically);
    uint32_t face{};
    if (m_isCubemap) {
        glGenTextures(1, &m_id);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_id);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    } else {
        glCreateTextures(GL_TEXTURE_2D, 1, &m_id);
        glTextureParameteri(m_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTextureParameteri(m_id, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(m_id, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }
    for (std::string_view filepath : m_filepaths) {
        stbi_uc* data = stbi_load(filepath.data(), &width, &height, &channels, 0);
        if (not data) [[unlikely]] {
            stbi_image_free(data);
            throw std::runtime_error("Failed to load texture: " + std::string(filepath) + " " + stbi_failure_reason());
        }
        m_width = width;
        m_height = height;

        if (channels == 4) {
            m_glFormat = GL_RGBA8;
            m_format = GL_RGBA;
        } else if (channels == 3) {
            m_glFormat = GL_RGB8;
            m_format = GL_RGB;
            // https://stackoverflow.com/questions/71284184/opengl-distorted-texture
            if (3 * width % 4 == 0) {
                glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
            } else {
                glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            }
        } else if (channels == 2) {
            m_glFormat = GL_RG8;
            m_format = GL_RG;
        } else if (channels == 1) {
            m_glFormat = GL_R8;
            m_format = GL_RED;
        } else [[unlikely]] {
            stbi_image_free(data);
            throw std::runtime_error("Texture format not supported: " + std::string(filepath) + " " + std::to_string(channels) + " channels");
        }

        if (m_isCubemap) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, m_format, m_width, m_height, 0, m_format, GL_UNSIGNED_BYTE, data);
            ++face;
        } else {
            glTextureStorage2D(m_id, m_mipmapLevel, m_glFormat, m_width, m_height);
            glTextureSubImage2D(m_id, 0, 0, 0, m_width, m_height, m_format, GL_UNSIGNED_BYTE, data);
            glGenerateTextureMipmap(m_id);
        }
        stbi_image_free(data);
    }
}

Texture::~Texture() {
#ifdef DEBUG
    std::string_view source = (m_filepaths.size() == 1) ? m_filepaths[0] : m_directory;
    CORE_INFO("Deleting texture {}: {}", m_id, source);
#endif
    glDeleteTextures(1, &m_id);
}

void Texture::bindSlot(uint32_t slot) {
    if (slot == 0) [[unlikely]] {
        throw std::runtime_error("Texture slot 0 is not allowed!");
    }
    m_slot = slot;
    glBindTextureUnit(slot, m_id);
}

void Texture::rebindSlot() {
    glBindTextureUnit(m_slot, m_id);
}

void Texture::unbindSlot() {
    glBindTextureUnit(m_slot, 0);  // unbind texture from slot
    m_slot = 0;                    // nothing render to this slot
}

std::unique_ptr<Texture> Texture::Create(uint32_t width, uint32_t height, GLenum glFormat, std::optional<bool> wrap) {
    return std::make_unique<Texture>(width, height, glFormat, wrap);
}
}
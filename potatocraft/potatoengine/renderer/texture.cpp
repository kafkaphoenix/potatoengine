
#include "potatoengine/renderer/texture.h"

#include <stb_image.h>

namespace potatoengine {

Texture::Texture(uint32_t width, uint32_t height)
    : m_width(width), m_height(height) {
    fprintf(stdout, "Creating empty texture\n");
    m_GLFormat = GL_RGBA8;
    m_format = GL_RGBA;

    glCreateTextures(GL_TEXTURE_2D, 1, &m_id);

    glTextureParameteri(m_id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(m_id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTextureStorage2D(m_id, 1, m_GLFormat, m_width, m_height);
}

Texture::Texture(const std::string& path)
    : m_path(path) {
    fprintf(stdout, "Loading texture\n");
    int width, height, channels;
    stbi_set_flip_vertically_on_load(1);
    stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
    if (!data) {
        fprintf(stdout, "Failed to load texture: %s\n", stbi_failure_reason());
        stbi_image_free(data);
        throw std::runtime_error("Failed to load texture: " + path);
    }

    m_loaded = true;
    m_width = width;
    m_height = height;

    if (channels == 4) {
        m_GLFormat = GL_RGBA8;
        m_format = GL_RGBA;
    } else if (channels == 3) {
        m_GLFormat = GL_RGB8;
        m_format = GL_RGB;
    } else {
        fprintf(stdout, "Texture format not supported!\n");
        stbi_image_free(data);
        throw std::runtime_error("Texture format not supported!");
    }

    glCreateTextures(GL_TEXTURE_2D, 1, &m_id);

    glTextureParameteri(m_id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(m_id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTextureStorage2D(m_id, 1, m_GLFormat, m_width, m_height);
    glTextureSubImage2D(m_id, 0, 0, 0, width, height, m_format, GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);

    glGenerateTextureMipmap(m_id);
}

Texture::~Texture() {
    glDeleteTextures(1, &m_id);
}

void Texture::bind(uint32_t slot) const {
    glBindTextureUnit(slot, m_id);
}

std::shared_ptr<Texture> Texture::Create(uint32_t width, uint32_t height) {
    return std::make_shared<Texture>(width, height);
}

std::shared_ptr<Texture> Texture::Create(const std::string& path) {
    return std::make_shared<Texture>(path);
}
}
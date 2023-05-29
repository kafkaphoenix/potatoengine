#pragma once

#include <glad/glad.h>

#include "potatoengine/pch.h"

namespace potatoengine {

class Texture {
   public:
    Texture(uint32_t width, uint32_t height);
    Texture(const std::string& path);
    ~Texture();

    uint32_t getWidth() const { return m_width; }
    uint32_t getHeight() const { return m_height; }
    uint32_t getId() const { return m_id; }

    const std::string& getPath() const { return m_path; }

    void setData(void* data);

    void bind(uint32_t slot = 0) const;

    bool isLoaded() const { return m_loaded; }

    bool operator==(const Texture& other) const {
        return m_id == other.getId();
    }
    static std::shared_ptr<Texture> Create(uint32_t width, uint32_t height);
    static std::shared_ptr<Texture> Create(const std::string& path);

   private:
    std::string m_path;
    bool m_loaded;
    uint32_t m_width, m_height;
    uint32_t m_id;
    GLenum m_GLFormat{}, m_format{};
};

}
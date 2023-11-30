#pragma once

#include "potatoengine/assets/texture.h"
#include "potatoengine/pch.h"

namespace potatoengine {

class FBO {
   public:
    FBO(uint32_t width, uint32_t height, uint32_t bufferType);
    ~FBO();

    void attachTexture();
    void attachDepthTexture();
    void attachDepthRenderBuffer();
    void attachStencilRenderBuffer();
    void attachDepthStencilRenderBuffer();
    void bindToDraw();
    void bindToRead();
    void unbind();
    void clear(const float color[4], const float depth);

    void resize(uint32_t width, uint32_t height);
    const std::unique_ptr<Texture>& getColorTexture() const noexcept { return m_colorTexture; }
    const std::unique_ptr<Texture>& getDepthTexture() const noexcept { return m_depthTexture; }
    uint32_t getBufferID() const noexcept;
    std::string_view getBufferType() const noexcept;
    const std::map<std::string, std::string>& getInfo();

    static std::unique_ptr<FBO> Create(uint32_t width, uint32_t height, uint32_t bufferType);

    static constexpr uint32_t DEPTH_TEXTURE = 1;
    static constexpr uint32_t DEPTH_RENDERBUFFER = 2;
    static constexpr uint32_t STENCIL_RENDERBUFFER = 3;
    static constexpr uint32_t DEPTH_STENCIL_RENDERBUFFER = 4;

   private:
    uint32_t m_id{};
    uint32_t m_width{};
    uint32_t m_height{};
    uint32_t m_depthBufferType{};
    std::unique_ptr<Texture> m_colorTexture{};
    std::unique_ptr<Texture> m_depthTexture{};
    uint32_t m_depthRenderBuffer{};
    uint32_t m_stencilRenderBuffer{};
    uint32_t m_depthStencilRenderBuffer{};

    std::map<std::string, std::string> m_info{};
};
}
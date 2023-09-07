#pragma once

#include <glm/glm.hpp>

#include "potatoengine/renderer/vao.h"

namespace potatoengine {
class RendererAPI {
   public:
    static void Init() noexcept;
    static void EnableCulling(bool enabled) noexcept;
    static void SetViewport(uint32_t x, uint32_t y, uint32_t w, uint32_t h) noexcept;
    static void SetClearColor(const glm::vec4& color) noexcept;
    static void SetWireframe(bool enabled) noexcept;
    static void Clear() noexcept;
    static void DrawIndexed(const std::shared_ptr<VAO>& vao) noexcept;
};
}
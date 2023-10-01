#pragma once

#include <glm/glm.hpp>

#include "potatoengine/renderer/vao.h"

namespace potatoengine {
class RendererAPI {
   public:
    static void Init() noexcept;
    static void SetCulling(bool enabled) noexcept;
    static void SetViewport(uint32_t x, uint32_t y, uint32_t w, uint32_t h) noexcept;
    static void SetClearColor(const float color[]) noexcept;
    static void SetClearDepth(const float depth) noexcept;
    static void SetWireframe(bool enabled) noexcept;
    static void SetBlend(bool enabled) noexcept;
    static void SetDepthTest(bool enabled) noexcept;
    static void SetDepthLEqual() noexcept;
    static void SetDepthLess() noexcept;
    static void Clear() noexcept;
    static void ClearColor() noexcept;
    static void ClearDepth() noexcept;
    static void DrawIndexed(const std::shared_ptr<VAO>& vao);
};
}
#pragma once

#include <glm/glm.hpp>

#include "pch.h"
#include "renderer/vao.h"

namespace potatoengine {
class RendererAPI {
  public:
    static void Init();
    static void ToggleCulling(bool enabled);
    static void SetViewport(uint32_t x, uint32_t y, uint32_t w, uint32_t h);
    static void SetClearColor(const std::array<float, 4>& color);
    static void SetClearDepth(const float depth);
    static void ToggleWireframe(bool enabled);
    static void ToggleBlend(bool enabled);
    static void ToggleDepthTest(bool enabled);
    static void SetDepthLEqual();
    static void SetDepthLess();
    static void Clear();
    static void ClearColor();
    static void ClearDepth();
    static void DrawIndexed(const std::shared_ptr<VAO>& vao);
};
}
#pragma once

#include <glm/glm.hpp>

#include "potatoengine/renderer/vao.h"

namespace potatoengine {
class RendererAPI {
   public:
    static void Init();
    static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
    static void SetClearColor(const glm::vec4& color);
    static void SetWireframe(bool enabled);
    static void Clear();
    static void DrawIndexed(const std::shared_ptr<VAO>& vao);
};
}
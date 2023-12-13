#pragma once

#include <glm/glm.hpp>

namespace potatoengine {

class Input {
  public:
    static bool IsKeyPressed(uint32_t k) noexcept;

    static bool IsMouseButtonPressed(uint32_t b) noexcept;
    static glm::vec2 GetMousePos() noexcept;
    static float GetMouseX() noexcept;
    static float GetMouseY() noexcept;
};
}
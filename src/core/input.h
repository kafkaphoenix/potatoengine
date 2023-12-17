#pragma once

#include <glm/glm.hpp>

namespace potatoengine {

class Input {
  public:
    static bool IsKeyPressed(uint32_t k);

    static bool IsMouseButtonPressed(uint32_t b);
    static glm::vec2 GetMousePos();
    static float GetMouseX();
    static float GetMouseY();
};
}
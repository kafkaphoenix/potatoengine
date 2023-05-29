#pragma once

#include <glm/glm.hpp>

namespace potatoengine {

class Input {
   public:
    static bool IsKeyPressed(uint32_t key);

    static bool IsMouseButtonPressed(uint32_t button);
    static glm::vec2 GetMousePosition();
    static float GetMouseX();
    static float GetMouseY();
};
}
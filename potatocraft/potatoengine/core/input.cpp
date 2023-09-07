#include "potatoengine/core/input.h"

#include <GLFW/glfw3.h>

#include "potatoengine/core/application.h"
#include "potatoengine/pch.h"

namespace potatoengine {

bool Input::IsKeyPressed(uint32_t k) noexcept {
    auto* w = Application::Get().getWindow().getNativeWindow();
    auto status = glfwGetKey(w, k);
    return status == GLFW_PRESS;
}

bool Input::IsMouseButtonPressed(uint32_t b) noexcept {
    auto* w = Application::Get().getWindow().getNativeWindow();
    auto status = glfwGetMouseButton(w, b);
    return status == GLFW_PRESS;
}

glm::vec2 Input::GetMousePos() noexcept {
    auto* w = Application::Get().getWindow().getNativeWindow();
    double xpos, ypos;
    glfwGetCursorPos(w, &xpos, &ypos);

    return glm::vec2((float)xpos, (float)ypos);
}

float Input::GetMouseX() noexcept { return GetMousePos().x; }

float Input::GetMouseY() noexcept { return GetMousePos().y; }

}
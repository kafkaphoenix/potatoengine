#include "potatoengine/core/input.h"

#include <GLFW/glfw3.h>

#include "potatoengine/core/application.h"
#include "potatoengine/pch.h"

namespace potatoengine {

bool Input::IsKeyPressed(const uint32_t key) {
    auto* window = Application::Get().getWindow().getNativeWindow();
    auto state = glfwGetKey(window, key);
    return state == GLFW_PRESS;
}

bool Input::IsMouseButtonPressed(const uint32_t button) {
    auto* window = Application::Get().getWindow().getNativeWindow();
    auto state = glfwGetMouseButton(window, button);
    return state == GLFW_PRESS;
}

glm::vec2 Input::GetMousePosition() {
    auto* window = Application::Get().getWindow().getNativeWindow();
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    return glm::vec2((float)xpos, (float)ypos);
}

float Input::GetMouseX() { return GetMousePosition().x; }

float Input::GetMouseY() { return GetMousePosition().y; }

}
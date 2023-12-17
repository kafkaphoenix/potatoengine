#include "core/input.h"

#include <GLFW/glfw3.h>

#include "core/application.h"
#include "pch.h"

namespace potatoengine {

bool Input::IsKeyPressed(uint32_t k) {
  GLFWwindow* w = Application::Get().getWindow().getNativeWindow();
  return glfwGetKey(w, k) == GLFW_PRESS;
}

bool Input::IsMouseButtonPressed(uint32_t b) {
  GLFWwindow* w = Application::Get().getWindow().getNativeWindow();
  return glfwGetMouseButton(w, b) == GLFW_PRESS;
}

glm::vec2 Input::GetMousePos() {
  GLFWwindow* w = Application::Get().getWindow().getNativeWindow();
  double xpos, ypos;
  glfwGetCursorPos(w, &xpos, &ypos);

  return glm::vec2((float)xpos, (float)ypos);
}

float Input::GetMouseX() { return GetMousePos().x; }

float Input::GetMouseY() { return GetMousePos().y; }

}
#pragma once

#include <imgui.h>

#include "core/application.h"

namespace potatoengine::ui {

inline void renderScene(uint32_t fboTextureID, std::string_view title,
                        glm::vec2 size, glm::vec2 position, bool fitToWindow) {
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
  ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver);
  auto& window = Application::Get().getWindow();
  int windowWidth = size.x != 0 ? size.x : window.getWidth();
  int windowHeight = size.y != 0 ? size.y : window.getHeight();
  windowWidth *= 0.75f;
  windowHeight *= 0.75f;
  ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight));
  int w, h;
  if (fitToWindow) {
    w = windowWidth;
    h = windowHeight;
  } else {
    w = window.getWidth();
    h = window.getHeight();
  }
  ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoBringToFrontOnFocus |
                                  ImGuiWindowFlags_NoResize |
                                  ImGuiWindowFlags_NoScrollbar;
  ImGui::Begin(title.data(), nullptr, window_flags);
  ImGui::Image((void*)fboTextureID, ImVec2(w, h), ImVec2(0, 1), ImVec2(1, 0));

  ImGui::PopStyleVar();
  ImGui::End();
}
}
#pragma once

#include <imgui.h>

#include "core/application.h"
#include "pch.h"

namespace potatoengine {
inline void renderScene(uint32_t fboTextureID, std::string_view title,
                        glm::vec2 size, glm::vec2 position, bool fitToWindow) {
  const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
  uint32_t windowWidth = size.x != 0 ? size.x : main_viewport->Size.x;
  uint32_t windowHeight = size.y != 0 ? size.y : main_viewport->Size.y;
  windowWidth *= 0.75f;
  windowHeight *= 0.75f;
  uint32_t w, h;
  if (fitToWindow) {
    w = windowWidth;
    h = windowHeight;
  } else {
    w = main_viewport->Size.x;
    h = main_viewport->Size.y;
  }
  ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoBringToFrontOnFocus |
                                  ImGuiWindowFlags_NoResize |
                                  ImGuiWindowFlags_NoScrollbar;
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
  ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver);
  ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight));
  if (ImGui::Begin(title.data(), nullptr, window_flags)) {
    ImGui::Image((void*)fboTextureID, ImVec2(w, h), ImVec2(0, 1), ImVec2(1, 0));
  }
  ImGui::PopStyleVar();
  ImGui::End();
}
}
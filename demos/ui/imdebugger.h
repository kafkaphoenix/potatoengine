#pragma once

#include "engineAPI.h"
#include "ui/immenubar.h"
#include "ui/imtabbar.h"

namespace demos {

void drawDebugger(std::weak_ptr<engine::AssetsManager> am,
                  std::weak_ptr<engine::Renderer> r, engine::SceneManager& sm,
                  Settings& settings) {
  const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
  ImGui::SetNextWindowPos(
    ImVec2(main_viewport->WorkPos.x + 650, main_viewport->WorkPos.y + 20),
    ImGuiCond_FirstUseEver);

  ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar;

  ImGui::SetNextWindowSize(ImVec2(500, 600), ImGuiCond_FirstUseEver);
  if (ImGui::Begin("Debugger", nullptr, window_flags)) {
    drawMenuBar(am, r, sm, settings);
    drawTabBar(am, r, sm, settings);
  }
  ImGui::End();
}
}
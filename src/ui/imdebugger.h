#pragma once

#include <imgui.h>

#include "assets/assetsManager.h"
#include "core/application.h"
#include "pch.h"
#include "renderer/renderer.h"
#include "scene/sceneManager.h"
#include "settings.h"
#include "ui/immanagersbar.h"
#include "ui/immenubar.h"

namespace potatoengine::ui {
void drawDebugger(const std::unique_ptr<Settings>& settings,
                  const std::unique_ptr<assets::AssetsManager>& assets_manager,
                  const std::unique_ptr<Renderer>& renderer,
                  const std::unique_ptr<SceneManager>& scene_manager) {
  ImGui::SetNextWindowPos(ImVec2(settings->windowWidth / 2.f, 20.f),
                          ImGuiCond_FirstUseEver);

  ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar;

  ImGui::SetNextWindowSize(ImVec2(500, 600), ImGuiCond_FirstUseEver);
  if (ImGui::Begin("Debugger", nullptr, window_flags)) {
    drawMenuBar(assets_manager, renderer, scene_manager, settings);
    drawManagersBar(assets_manager, renderer, scene_manager, settings);
  }
  ImGui::End();
}
}
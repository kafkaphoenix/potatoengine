#pragma once

#include <imgui.h>

#include "assets/assetsManager.h"
#include "core/application.h"
#include "core/settingsManager.h"
#include "core/statesManager.h"
#include "pch.h"
#include "render/renderManager.h"
#include "scene/sceneManager.h"
#include "ui/immanagersbar.h"
#include "ui/immenubar.h"

namespace potatoengine::ui {

inline void
drawDebugger(const std::unique_ptr<SettingsManager>& settings_manager,
             const std::unique_ptr<assets::AssetsManager>& assets_manager,
             const std::unique_ptr<RenderManager>& render_manager,
             const std::unique_ptr<SceneManager>& scene_manager,
             const std::unique_ptr<StatesManager>& states_manager) {
  ImGui::SetNextWindowPos(ImVec2(settings_manager->windowWidth / 2.f, 20.f),
                          ImGuiCond_FirstUseEver);

  ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar;

  ImGui::SetNextWindowSize(ImVec2(500, 600), ImGuiCond_FirstUseEver);
  if (ImGui::Begin("Debugger", nullptr, window_flags)) {
    drawMenuBar(assets_manager, render_manager, scene_manager,
                settings_manager);
    drawManagersBar(assets_manager, render_manager, scene_manager,
                    settings_manager, states_manager);
  }
  ImGui::End();
}
}
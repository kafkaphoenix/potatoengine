#pragma once

#include <imgui.h>

#include "assets/assetsManager.h"
#include "core/settingsManager.h"
#include "pch.h"
#include "render/renderManager.h"
#include "scene/sceneManager.h"
#include "ui/imabout.h"
#include "ui/imlogger.h"
#include "ui/immetrics.h"

namespace potatoengine::ui {

inline void
drawMenuBar(const std::unique_ptr<assets::AssetsManager>& assets_manager,
            const std::unique_ptr<RenderManager>& render_manager,
            const std::unique_ptr<SceneManager>& scene_manager,
            const std::unique_ptr<SettingsManager>& settings_manager) {
  drawMetrics(assets_manager, render_manager, scene_manager);
  drawLogger();
  drawAbout();

  if (ImGui::BeginMenuBar()) {
    if (ImGui::BeginMenu("Scenes")) {
      for (uint32_t n = 0; n < settings_manager->scenes.size(); n++) {
        if (ImGui::MenuItem(settings_manager->scenes[n], nullptr,
                            settings_manager->activeScene ==
                              settings_manager->scenes[n])) {
          if (settings_manager->activeScene not_eq
              settings_manager->scenes[n]) {
            settings_manager->activeScene = settings_manager->scenes[n];
            settings_manager->activeScenePath =
              settings_manager->scenesPaths[n];
            settings_manager->reloadScene = true;
          }
        }
      }
      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Tools")) {
      ImGui::MenuItem("Metrics", "CTRL+M", &show_tool_metrics);
      ImGui::MenuItem("Logger", "CTRL+L", &show_tool_logger);
      ImGui::MenuItem("About", NULL, &show_tool_about);
      ImGui::EndMenu();
    }
    ImGui::EndMenuBar();
  }
}
}
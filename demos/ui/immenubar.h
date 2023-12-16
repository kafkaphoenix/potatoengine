#pragma once

#include "engineAPI.h"
#include "settings.h" // linter
#include "ui/imabout.h"
#include "ui/imlogger.h"
#include "ui/immetrics.h"

namespace demos {

void drawMenuBar(std::weak_ptr<engine::AssetsManager> am,
                 std::weak_ptr<engine::Renderer> r, engine::SceneManager& sm,
                 Settings& settings) {
  drawMetrics(am, r, sm);
  drawLogger();
  drawAbout();

  if (ImGui::BeginMenuBar()) {
    if (ImGui::BeginMenu("Scenes")) {
      for (int n = 0; n < settings.scenes.size(); n++) {
        if (ImGui::MenuItem(settings.scenes[n], nullptr,
                            settings.activeScene == settings.scenes[n])) {
          if (settings.activeScene not_eq settings.scenes[n]) {
            settings.activeScene = settings.scenes[n];
            settings.activeScenePath = settings.scenesPaths[n];
            settings.reloadScene = true;
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
#pragma once

#include "engineAPI.h"
#include "settings.h" // linter
#include "ui/imabout.h"
#include "ui/imlogger.h"
#include "ui/immetrics.h"

namespace demos {

void drawMenuBar(std::weak_ptr<engine::AssetsManager> am, std::weak_ptr<engine::Renderer> r, engine::SceneManager& sm,
                 Settings& settings) {
  static bool show_tool_metrics = false;
  static bool show_tool_logger = false;
  static bool show_tool_about = false;

  drawMetrics(&show_tool_metrics, am, r, sm);
  drawLogger(&show_tool_logger);
  drawAbout(&show_tool_about);

  if (ImGui::BeginMenuBar()) {
    if (ImGui::BeginMenu("Scenes")) {
      for (int n = 0; n < settings.scenes.size(); n++) {
        if (ImGui::MenuItem(settings.scenes[n], nullptr, settings.activeScene == settings.scenes[n])) {
          if (settings.activeScene != settings.scenes[n]) {
            settings.activeScene = settings.scenes[n];
            settings.activeScenePath = settings.scenesPaths[n];
            settings.reloadScene = true;
          }
        }
      }
      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Tools")) {
      ImGui::MenuItem("Metrics", NULL, &show_tool_metrics);
      ImGui::MenuItem("Logger", NULL, &show_tool_logger);
      ImGui::MenuItem("About", NULL, &show_tool_about);
      ImGui::EndMenu();
    }
    ImGui::EndMenuBar();
  }
}
}
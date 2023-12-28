#pragma once

#include <imgui.h>

#include "assets/assetsManager.h"
#include "pch.h"
#include "renderer/renderer.h"
#include "scene/sceneManager.h"
#include "settings.h"
#include "ui/imabout.h"
#include "ui/imlogger.h"
#include "ui/immetrics.h"

namespace potatoengine::ui {
void drawMenuBar(std::weak_ptr<AssetsManager> am, std::weak_ptr<Renderer> r,
                 std::weak_ptr<SceneManager> sm, std::weak_ptr<Settings> s) {
  drawMetrics(am, r, sm);
  drawLogger();
  drawAbout();

  const auto& settings = s.lock();
  ENGINE_ASSERT(settings, "Settings is null!");

  if (ImGui::BeginMenuBar()) {
    if (ImGui::BeginMenu("Scenes")) {
      for (int n = 0; n < settings->scenes.size(); n++) {
        if (ImGui::MenuItem(settings->scenes[n], nullptr,
                            settings->activeScene == settings->scenes[n])) {
          if (settings->activeScene not_eq settings->scenes[n]) {
            settings->activeScene = settings->scenes[n];
            settings->activeScenePath = settings->scenesPaths[n];
            settings->reloadScene = true;
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
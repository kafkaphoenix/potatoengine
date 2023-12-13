#pragma once

#include "engineAPI.h"
#include "ui/imassets.h"
#include "ui/imentities.h"
#include "ui/imrenderer.h"
#include "ui/imsettings.h"

namespace demos {

void drawTabBar(std::weak_ptr<engine::AssetsManager> am, std::weak_ptr<engine::Renderer> r, engine::SceneManager& sm,
                Settings& settings) {
  if (ImGui::BeginTabBar("RootTabBar", ImGuiTabBarFlags_Reorderable)) {
    if (ImGui::BeginTabItem("Assets")) {
      drawAssets(am);
      ImGui::EndTabItem();
    } else {
      selectedAssetTabKey = "";
    }
    if (ImGui::BeginTabItem("Entities")) {
      drawEntities(sm);
      ImGui::EndTabItem();
    } else {
      selectedEntityTabKey = "";
    }
    if (ImGui::BeginTabItem("Renderer")) {
      drawRenderer(r);
      ImGui::EndTabItem();
    } else {
      selectedRendererTabKey = "";
    }
    if (ImGui::BeginTabItem("Settings")) {
      drawSettings(settings);
      ImGui::EndTabItem();
    } else {
      selectedSettingTabKey = "";
    }
    ImGui::EndTabBar();
  }
}
}
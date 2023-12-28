#pragma once

#include <imgui.h>

#include "assets/assetsManager.h"
#include "pch.h"
#include "renderer/renderer.h"
#include "scene/sceneManager.h"
#include "settings.h"
#include "ui/imassets.h"
#include "ui/imentities.h"
#include "ui/imrenderer.h"
#include "ui/imsettings.h"

namespace potatoengine::ui {
void drawTabBar(std::weak_ptr<engine::AssetsManager> am,
                std::weak_ptr<engine::Renderer> r,
                std::weak_ptr<engine::SceneManager> sm,
                std::weak_ptr<Settings> s) {
  if (ImGui::BeginTabBar("RootTabBar", ImGuiTabBarFlags_Reorderable)) {
    if (ImGui::BeginTabItem("Assets")) {
      drawAssets(am, s);
      ImGui::EndTabItem();
    } else {
      selectedAssetTabKey.clear();
      selectedAssetTabType.clear();
      selectedFilepath.clear();
      textFilterAssets[0] = '\0';
    }
    if (ImGui::BeginTabItem("Entities")) {
      drawEntities(sm, s);
      ImGui::EndTabItem();
    } else {
      selectedEntityTabKey.clear();
      textFilterEntities[0] = '\0';
      filterPrefabs = false;
      filterPrototypes = false;
      filterInstances = false;
      filterComponents = false;
    }
    if (ImGui::BeginTabItem("Renderer")) {
      drawRenderer(r, s);
      ImGui::EndTabItem();
    } else {
      selectedRendererTabKey.clear();
      selectedRendererTabType.clear();
      textFilterRenderer[0] = '\0';
      filterFBOS = false;
      filterShaderPrograms = false;
      filterShaderInfo = false;
    }
    if (ImGui::BeginTabItem("Settings")) {
      drawSettings(s);
      ImGui::EndTabItem();
    } else {
      selectedSettingTabKey.clear();
    }
    ImGui::EndTabBar();
  }
}
}
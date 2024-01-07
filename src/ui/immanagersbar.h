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
void drawManagersBar(const std::unique_ptr<AssetsManager>& assets_manager,
                     const std::unique_ptr<Renderer>& renderer,
                     const std::unique_ptr<SceneManager>& scene_manager,
                     const std::unique_ptr<Settings>& settings) {
  if (ImGui::BeginTabBar("RootTabBar", ImGuiTabBarFlags_Reorderable)) {
    if (ImGui::BeginTabItem("Assets Manager")) {
      drawAssetsManager(assets_manager, settings);
      ImGui::EndTabItem();
    } else {
      selectedAssetManagerTabKey.clear();
      selectedAssetTabType.clear();
      selectedFilepath.clear();
      assets_text_filter[0] = '\0';
    }
    if (ImGui::BeginTabItem("Scene Manager")) {
      drawSceneManager(scene_manager, settings);
      ImGui::EndTabItem();
    } else {
      selectedSceneManagerTabKey.clear();
      scene_objects_text_filter[0] = '\0';
      filterPrefabs = false;
      filterPrototypes = false;
      filterInstances = false;
      filterComponents = false;
    }
    if (ImGui::BeginTabItem("Render Manager")) {
      drawRenderManager(renderer, settings);
      ImGui::EndTabItem();
    } else {
      selectedRenderManagerTabKey.clear();
      selectedRenderManagerTabType.clear();
      render_objects_text_filter[0] = '\0';
      filterFBOS = false;
      filterShaderPrograms = false;
      filterShaderInfo = false;
    }
    if (ImGui::BeginTabItem("Settings Manager")) {
      drawSettingsManager(settings, renderer);
      ImGui::EndTabItem();
    } else {
      selectedSettingsManagerTabKey.clear();
    }
    ImGui::EndTabBar();
  }
}
}
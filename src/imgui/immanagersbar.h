#pragma once

#include <imgui.h>

#include "assets/assetsManager.h"
#include "core/settingsManager.h"
#include "core/statesManager.h"
#include "pch.h"
#include "render/renderManager.h"
#include "scene/sceneManager.h"
#include "imgui/im_assets_manager.h"
#include "imgui/im_render_manager.h"
#include "imgui/im_scene_manager.h"
#include "imgui/im_settings_manager.h"
#include "imgui/im_states_manager.h"

namespace potatoengine {

inline void
drawManagersBar(const std::unique_ptr<assets::AssetsManager>& assets_manager,
                const std::unique_ptr<RenderManager>& render_manager,
                const std::unique_ptr<SceneManager>& scene_manager,
                const std::unique_ptr<SettingsManager>& settings_manager,
                const std::unique_ptr<StatesManager>& states_manager) {
  if (ImGui::BeginTabBar("RootTabBar", ImGuiTabBarFlags_Reorderable)) {
    if (ImGui::BeginTabItem("Assets Manager")) {
      drawAssetsManager(assets_manager, settings_manager);
      ImGui::EndTabItem();
    } else {
      selectedAssetManagerTabKey.clear();
      selectedAssetTabType.clear();
      selectedFilepath.clear();
      assets_text_filter[0] = '\0';
    }
    if (ImGui::BeginTabItem("Scene Manager")) {
      drawSceneManager(scene_manager, settings_manager);
      ImGui::EndTabItem();
    } else {
      selectedSceneManagerTabKey.clear();
      scene_objects_text_filter[0] = '\0';
      filterPrefabs = false;
      filterPrototypes = false;
      filterInstances = false;
      filterComponents = false;
      filterSystems = false;
    }
    if (ImGui::BeginTabItem("Render Manager")) {
      drawRenderManager(render_manager, settings_manager);
      ImGui::EndTabItem();
    } else {
      selectedRenderManagerTabKey.clear();
      selectedRenderManagerTabType.clear();
      render_objects_text_filter[0] = '\0';
      filterFBOS = false;
      filterShaderPrograms = false;
      filterShaderInfo = false;
    }
    if (ImGui::BeginTabItem("States Manager")) {
      drawStatesManager(settings_manager, states_manager);
      ImGui::EndTabItem();
    } else {
      states_text_filter[0] = '\0';
      filterStates = false;
      filterLayers = false;
    }
    if (ImGui::BeginTabItem("Settings Manager")) {
      drawSettingsManager(settings_manager, render_manager);
      ImGui::EndTabItem();
    } else {
      selectedSettingsManagerTabKey.clear();
    }
    ImGui::EndTabBar();
  }
}
}
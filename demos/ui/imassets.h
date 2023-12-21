#pragma once

#include "engineAPI.h"

namespace demos {

std::string selectedAssetTabKey;
std::string selectedAssetTabType;
std::string selectedFilepath;
char textFilterAssets[128]{}; // TODO: move to class

void drawAssets(std::weak_ptr<engine::AssetsManager> am) {
  auto manager = am.lock();
  APP_ASSERT(manager, "AssetsManager is null!");

  const auto& assets = manager->getAssetsByType();

  if (assets.empty()) {
    ImGui::Text("No assets loaded");
    return;
  }

  int collapsed = engine::ui::collapser();

  ImGui::InputText("##filter", textFilterAssets,
                   IM_ARRAYSIZE(textFilterAssets));
  if (ImGui::IsItemHovered()) {
    ImGui::SetTooltip("Filter assets by name");
  }
  ImGui::SameLine();
  if (ImGui::Button("Clear Filter")) {
    textFilterAssets[0] = '\0';
  }

  ImGui::Separator();
  ImGui::Columns(2);

  bool filterOption = false;
  for (const auto& [AssetType, AssetsData] : assets) {
    if (collapsed not_eq -1)
      ImGui::SetNextItemOpen(collapsed not_eq 0);

    if (ImGui::CollapsingHeader(AssetType.c_str())) {
      for (const auto& [AssetName, _] : AssetsData) {
        if (textFilterAssets[0] not_eq '\0' and
            strstr(AssetName.c_str(), textFilterAssets) == nullptr) {
          continue;
        }
        if (ImGui::Selectable(AssetName.c_str())) {
          selectedAssetTabKey = AssetName;
          selectedAssetTabType = AssetType;
        }
      }
    }
  }

  if (collapsed == 0) {
    selectedAssetTabKey = "";
    selectedAssetTabType = "";
    selectedFilepath = "";
  }

  ImGui::NextColumn();
  if (not selectedAssetTabKey.empty()) {
    const auto& asset = assets.at(selectedAssetTabType).at(selectedAssetTabKey);
    const auto& assetInfo =
      std::visit([](const auto& asset) { return asset->getInfo(); }, asset);

    for (const auto& [key, value] : assetInfo) {
      if (key.starts_with("Targeted Prototype ") and
          selectedAssetTabType == "Prefab") {
        const auto& prefab = manager->get<engine::Prefab>(selectedAssetTabKey);
        const auto& prototypeInfo = prefab->getTargetedPrototypeInfo(value);
        if (ImGui::TreeNode(
              (selectedAssetTabType + selectedAssetTabKey + key).c_str(),
              key.c_str())) {
          for (const auto& [key, value] : prototypeInfo) {
            ImGui::BulletText("%s: %s", key.c_str(), value.c_str());
          }
          ImGui::TreePop();
        }
      } else if (key.starts_with("Loaded Texture ") and
                 selectedAssetTabType == "Model") {
        const auto& model = manager->get<engine::Model>(selectedAssetTabKey);
        const auto& textureInfo = model->getLoadedTextureInfo(value);
        if (ImGui::TreeNode(
              (selectedAssetTabType + selectedAssetTabKey + key).c_str(),
              key.c_str())) {
          for (const auto& [key, value] : textureInfo) {
            ImGui::BulletText("%s: %s", key.c_str(), value.c_str());
          }
          ImGui::TreePop();
        }
      } else if (key.starts_with("Filepath ") and
                 selectedAssetTabType == "Texture") {
        const auto& texture =
          manager->get<engine::Texture>(selectedAssetTabKey);
        if (texture->isCubemap()) {
          ImGui::BulletText("%s: %s", key.c_str(), value.c_str());
        } else {
          ImGui::BulletText("%s: %s", key.c_str(), value.c_str());
          if (ImGui::IsItemHovered()) {
            selectedFilepath = value;
          } else {
            selectedFilepath = "";
          }
        }
      } else {
        ImGui::BulletText("%s: %s", key.c_str(), value.c_str());
      }
    }

    if (not selectedFilepath.empty()) {
      const auto& texture = manager->get<engine::Texture>(selectedAssetTabKey);
      if (not texture->isCubemap()) {
        ImGui::Image((void*)texture->getID(), ImVec2(64, 64));
      }
    }
  }

  ImGui::Columns(1);
}
}

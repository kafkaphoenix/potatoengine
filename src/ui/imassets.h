#pragma once

#include <imgui.h>

#include "assets/assetsManager.h"
#include "assets/model.h"
#include "assets/prefab.h"
#include "assets/texture.h"
#include "pch.h"
#include "settings.h"
#include "ui/imutils.h"

namespace potatoengine::ui {

std::string selectedAssetTabKey;
std::string selectedAssetTabType;
std::string selectedFilepath;
char textFilterAssets[128]{}; // TODO: move to class

void drawAssets(std::weak_ptr<AssetsManager> am, std::weak_ptr<Settings> s) {
  const auto& assetsManager = am.lock();
  ENGINE_ASSERT(assetsManager, "AssetsManager is null!");

  const auto& settings = s.lock();
  ENGINE_ASSERT(settings, "Settings is null!");

  const auto& assets = assetsManager->getAssetsByType();

  if (assets.empty()) {
    ImGui::Text("No assets loaded");
    return;
  }

  int collapsed = collapser();

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
    if (collapsed not_eq -1) {
      ImGui::SetNextItemOpen(collapsed not_eq 0);
    }

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

  if (collapsed == 0 or settings->reloadScene) {
    selectedAssetTabKey.clear();
    selectedAssetTabType.clear();
    selectedFilepath.clear();
  }

  ImGui::NextColumn();
  if (not selectedAssetTabKey.empty()) {
    const auto& asset = assets.at(selectedAssetTabType).at(selectedAssetTabKey);
    const auto& assetInfo =
      std::visit([](const auto& asset) { return asset->getInfo(); }, asset);

    for (const auto& [key, value] : assetInfo) {
      if (key.starts_with("Prototype ") and selectedAssetTabType == "Prefab") {
        const auto& prefab = assetsManager->get<Prefab>(selectedAssetTabKey);
        const auto& prototypeInfo = prefab->getTargetedPrototypeInfo(value);
        if (ImGui::TreeNode(
              (selectedAssetTabType + selectedAssetTabKey + key + settings->activeScene).c_str(),
              key.c_str())) {
          for (const auto& [key, value] : prototypeInfo) {
            ImGui::BulletText("%s: %s", key.c_str(), value.c_str());
          }
          ImGui::TreePop();
        }
      } else if (key.starts_with("Loaded Texture ") and
                 selectedAssetTabType == "Model") {
        const auto& model = assetsManager->get<Model>(selectedAssetTabKey);
        const auto& textureInfo = model->getLoadedTextureInfo(value);
        if (ImGui::TreeNode(
              (selectedAssetTabType + selectedAssetTabKey + key + settings->activeScene).c_str(),
              key.c_str())) {
          for (const auto& [key, value] : textureInfo) {
            ImGui::BulletText("%s: %s", key.c_str(), value.c_str());
          }
          ImGui::TreePop();
        }
      } else if (key.starts_with("Filepath ") and
                 selectedAssetTabType == "Texture") {
        const auto& texture = assetsManager->get<Texture>(selectedAssetTabKey);
        if (texture->isCubemap()) {
          ImGui::BulletText("%s: %s", key.c_str(), value.c_str());
        } else {
          ImGui::BulletText("%s: %s", key.c_str(), value.c_str());
          if (ImGui::IsItemHovered()) {
            selectedFilepath = value;
          } else {
            selectedFilepath.clear();
          }
        }
      } else {
        ImGui::BulletText("%s: %s", key.c_str(), value.c_str());
      }
    }

    if (not selectedFilepath.empty()) {
      const auto& texture = assetsManager->get<Texture>(selectedAssetTabKey);
      if (not texture->isCubemap()) {
        int maxWidth = texture->getWidth() > 128 ? 128 : texture->getWidth();
        int maxHeight = texture->getHeight() > 128 ? 128 : texture->getHeight();
        ImGui::Image((void*)texture->getID(), ImVec2(maxWidth, maxHeight));
      }
    }
  }

  ImGui::Columns(1);
}
}

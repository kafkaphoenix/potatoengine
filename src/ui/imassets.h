#pragma once

#include <imgui.h>

#include "assets/assetsManager.h"
#include "assets/model.h"
#include "assets/prefab.h"
#include "assets/texture.h"
#include "pch.h"
#include "settings.h"
#include "ui/imutils.h"

using namespace potatoengine::assets;

namespace potatoengine::ui {

std::string selectedAssetManagerTabKey;
std::string selectedAssetTabType;
std::string selectedFilepath;
char assets_text_filter[128]{}; // TODO: move to class

void drawAssetsManager(const std::unique_ptr<AssetsManager>& assets_manager,
                       const std::unique_ptr<Settings>& settings) {
  const auto& assets = assets_manager->getAssets();

  if (assets.empty()) {
    ImGui::Text("No assets loaded");
    return;
  }

  int collapsed = collapser();

  ImGui::InputText("##filter", assets_text_filter,
                   IM_ARRAYSIZE(assets_text_filter));
  if (ImGui::IsItemHovered()) {
    ImGui::SetTooltip("Filter assets by name");
  }
  ImGui::SameLine();
  if (ImGui::Button("Clear Filter")) {
    assets_text_filter[0] = '\0';
  }

  ImGui::Separator();
  ImGui::Columns(2);

  bool filterOption = false;
  for (const auto& [type, value] : assets) {
    if (collapsed not_eq -1) {
      ImGui::SetNextItemOpen(collapsed not_eq 0);
    }

    if (ImGui::CollapsingHeader(type.c_str())) {
      for (const auto& [name, _] : value) {
        if (assets_text_filter[0] not_eq '\0' and
            strstr(name.c_str(), assets_text_filter) == nullptr) {
          continue;
        }
        if (ImGui::Selectable(name.c_str())) {
          selectedAssetManagerTabKey = name;
          selectedAssetTabType = type;
        }
      }
    }
  }

  if (collapsed == 0 or settings->reloadScene) {
    selectedAssetManagerTabKey.clear();
    selectedAssetTabType.clear();
    selectedFilepath.clear();
  }

  ImGui::NextColumn();
  if (not selectedAssetManagerTabKey.empty()) {
    const auto& asset =
      assets.at(selectedAssetTabType).at(selectedAssetManagerTabKey);
    const auto& assetInfo = asset->getInfo();

    for (const auto& [key, value] : assetInfo) {
      if (key.starts_with("Prototype ") and selectedAssetTabType == "Prefab") {
        const auto& prefab =
          assets_manager->get<Prefab>(selectedAssetManagerTabKey);
        const auto& prototypeInfo = prefab->getTargetedPrototypeInfo(value);
        if (ImGui::TreeNode((selectedAssetTabType + selectedAssetManagerTabKey +
                             key + settings->activeScene)
                              .c_str(),
                            key.c_str())) {
          for (const auto& [key, value] : prototypeInfo) {
            ImGui::BulletText("%s: %s", key.c_str(), value.c_str());
          }
          ImGui::TreePop();
        }
      } else if (key.starts_with("Loaded Texture ") and
                 selectedAssetTabType == "Model") {
        const auto& model =
          assets_manager->get<Model>(selectedAssetManagerTabKey);
        const auto& textureInfo = model->getLoadedTextureInfo(value);
        if (ImGui::TreeNode((selectedAssetTabType + selectedAssetManagerTabKey +
                             key + settings->activeScene)
                              .c_str(),
                            key.c_str())) {
          for (const auto& [key, value] : textureInfo) {
            ImGui::BulletText("%s: %s", key.c_str(), value.c_str());
          }
          ImGui::TreePop();
        }
      } else if (key.starts_with("Filepath ") and
                 selectedAssetTabType == "Texture") {
        const auto& texture =
          assets_manager->get<Texture>(selectedAssetManagerTabKey);
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
      const auto& texture =
        assets_manager->get<Texture>(selectedAssetManagerTabKey);
      if (not texture->isCubemap()) {
        int maxWidth = texture->getWidth() > 128 ? 128 : texture->getWidth();
        int maxHeight = texture->getHeight() > 128 ? 128 : texture->getHeight();
        ImGui::Image((void*)texture->getID(), ImVec2(maxWidth, maxHeight), ImVec2(0, 1),
                     ImVec2(1, 0));
      }
    }
  }

  ImGui::Columns(1);
}
}

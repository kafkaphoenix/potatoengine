#pragma once

#include "potatoengine/engineAPI.h"

namespace demos {

std::string selectedAssetTabKey;
std::string selectedAssetTabType;

void drawAssets(std::weak_ptr<engine::AssetsManager> am) {
    auto manager = am.lock();
    APP_ASSERT(manager, "AssetsManager is null!");

    const auto& assets = manager->getAssetsByType();

    if (assets.empty()) {
        ImGui::Text("No assets loaded");
        return;
    }

    ImGui::Columns(2);

    int collapsed = engine::ui::collapser();

    for (const auto& [AssetType, AssetsData] : assets) {
        if (collapsed != -1)
            ImGui::SetNextItemOpen(collapsed != 0);

        if (ImGui::CollapsingHeader(AssetType.c_str())) {
            for (const auto& [AssetName, _] : AssetsData) {
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
    }

    ImGui::NextColumn();
    if (not selectedAssetTabKey.empty()) {
        const auto& asset = assets.at(selectedAssetTabType).at(selectedAssetTabKey);
        const auto& assetInfo = std::visit([](const auto& asset) { return asset->getInfo(); }, asset);

        for (const auto& [key, value] : assetInfo) {
            if (key.contains("Targeted Prototype") and selectedAssetTabType == "Prefab") {
                const auto& prefab = manager->get<engine::Prefab>(selectedAssetTabKey);
                const auto& prototypeInfo = prefab->getTargetedPrototypeInfo(value);
                if (ImGui::TreeNode(key.c_str())) {
                    for (const auto& [key, value] : prototypeInfo) {
                        ImGui::BulletText("%s: %s", key.c_str(), value.c_str());
                    }
                    ImGui::TreePop();
                }
            } else if (key.contains("Loaded Texture") and selectedAssetTabType == "Model") {
                const auto& model = manager->get<engine::Model>(selectedAssetTabKey);
                const auto& textureInfo = model->getLoadedTextureInfo(value);
                if (ImGui::TreeNode(key.c_str())) {
                    for (const auto& [key, value] : textureInfo) {
                        ImGui::BulletText("%s: %s", key.c_str(), value.c_str());
                    }
                    ImGui::TreePop();
                }
            } else {
                ImGui::BulletText("%s: %s", key.c_str(), value.c_str());
            }
        }
    }

    ImGui::Columns(1);
}
}

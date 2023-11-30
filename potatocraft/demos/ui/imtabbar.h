#pragma once

#include "demos/ui/imassets.h"
#include "demos/ui/imentities.h"
#include "demos/ui/imrenderer.h"
#include "demos/ui/imsettings.h"
#include "potatoengine/engineAPI.h"

namespace demos {

void drawTabBar(std::weak_ptr<engine::AssetsManager> am, std::weak_ptr<engine::Renderer> r, engine::SceneManager& sm, Settings& settings) {
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
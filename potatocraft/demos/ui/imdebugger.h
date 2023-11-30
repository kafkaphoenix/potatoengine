#pragma once

#include "demos/ui/immenubar.h"
#include "demos/ui/imtabbar.h"
#include "potatoengine/engineAPI.h"

namespace demos {

void drawDebugger(std::weak_ptr<engine::AssetsManager> am, std::weak_ptr<engine::Renderer> r, engine::SceneManager& sm, Settings& settings) {
    const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + 650, main_viewport->WorkPos.y + 20), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(550, 680), ImGuiCond_FirstUseEver);

    if (ImGui::Begin("Debugger", nullptr, ImGuiWindowFlags_MenuBar)) {
        drawMenuBar(am, r, sm, settings);
        drawTabBar(am, r, sm, settings);
    }
    ImGui::End();
}
}
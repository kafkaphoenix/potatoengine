#pragma once

#include <imgui.h>

#include "assets/assetsManager.h"
#include "pch.h"
#include "renderer/renderer.h"
#include "scene/sceneManager.h"
#include "settings.h"
#include "ui/immenubar.h"
#include "ui/imtabbar.h"

namespace potatoengine::ui {
void drawDebugger(std::weak_ptr<AssetsManager> am, std::weak_ptr<Renderer> r,
                  std::weak_ptr<SceneManager> sm, std::weak_ptr<Settings> s) {
  const auto& settings = s.lock();
  ENGINE_ASSERT(settings, "Settings is null!");

  ImGui::SetNextWindowPos(ImVec2(settings->windowWidth / 2.f, 20.f),
                          ImGuiCond_FirstUseEver);

  ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar;

  ImGui::SetNextWindowSize(ImVec2(500, 600), ImGuiCond_FirstUseEver);
  if (ImGui::Begin("Debugger", nullptr, window_flags)) {
    drawMenuBar(am, r, sm, s);
    drawTabBar(am, r, sm, s);
  }
  ImGui::End();
}
}
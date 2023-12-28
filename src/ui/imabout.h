#pragma once

#include <imgui.h>

#include "ui/imutils.h"

namespace potatoengine::ui {

bool show_tool_about = false;

void drawAbout() {
  if (not show_tool_about)
    return;

  ImGui::SetNextWindowSize(ImVec2(400, 400), ImGuiCond_FirstUseEver);
  if (ImGui::Begin("About PotatoEngine", &show_tool_about)) {
    ImGui::Text("Currently in development and is not yet ready for production");
    ImGui::Separator();
    ImGui::Text(
      "PotatoEngine is open source and licensed under the MIT license");
    ImGui::Text("Source code and Documentation can be found on");
    ImGui::SameLine();
    url("Github", "https://github.com/kafkaphoenix/potatoengine.git");
  }
  ImGui::End();
}
}
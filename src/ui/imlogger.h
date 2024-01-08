#pragma once

#include <imgui.h>

#include "core/logManager.h"
#include "pch.h"
#include "ui/imguiLogSink.h"

namespace potatoengine::ui {

bool show_tool_logger = false;

void drawLogger() {
  if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_L)) and
      ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_LeftCtrl))) {
    show_tool_logger = not show_tool_logger;
  }

  if (not show_tool_logger)
    return;

  auto sink =
    dynamic_cast<ImGuiLogSink*>(LogManager::GetEngineLogger()->sinks()[1].get());
  sink->Draw(&show_tool_logger);
}
}

#pragma once

#include "engineAPI.h"

namespace demos {

void drawLogger(bool* show_tool_logger) {
  if (not *show_tool_logger)
    return;

  auto sink = dynamic_cast<engine::ui::ImGuiLogSink*>(engine::LogManager::GetAppLogger()->sinks()[1].get());
  sink->Draw();
}
}

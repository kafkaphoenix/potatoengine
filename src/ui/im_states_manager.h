#pragma once

#include <imgui.h>

#include "core/application.h"
#include "pch.h"

namespace potatoengine::ui {

char states_text_filter[128]{}; // TODO: move to class
bool filterStates{};

inline void drawStatesManager() {
  ImGui::InputText("##filter", states_text_filter,
                   IM_ARRAYSIZE(states_text_filter));
  if (ImGui::IsItemHovered()) {
    ImGui::SetTooltip("Filter states by name");
  }
  ImGui::SameLine();
  if (ImGui::Button("Clear Filter")) {
    states_text_filter[0] = '\0';
  }

  ImGui::Separator();

  for (const auto& [key, value] :
       Application::Get().getStatesManager()->getMetrics()) {
    if (states_text_filter[0] not_eq '\0' and
        strstr(key.c_str(), states_text_filter) == nullptr) {
      continue;
    }
    ImGui::BulletText("%s: %s", key.c_str(), value.c_str());
  }
}
}

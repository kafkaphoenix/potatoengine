#pragma once

#include <imgui.h>

#include "core/settingsManager.h"
#include "core/statesManager.h"
#include "pch.h"
#include "ui/imutils.h"
#include "utils/mapJsonSerializer.h"
#include "utils/numericComparator.h"

namespace potatoengine::ui {

char states_text_filter[128]{}; // TODO: move to class
bool filterStates{};
bool filterLayers{};
std::map<std::string, std::string> selectedLayerMetrics;

inline void
drawStatesManager(const std::unique_ptr<SettingsManager>& settings_manager,
                  const std::unique_ptr<StatesManager>& states_manager) {
  int collapsed = collapser();

  ImGui::InputText("##filter", states_text_filter,
                   IM_ARRAYSIZE(states_text_filter));
  if (ImGui::IsItemHovered()) {
    ImGui::SetTooltip("Filter states or layers by name");
  }
  ImGui::SameLine();
  if (ImGui::Button("Clear Filter")) {
    states_text_filter[0] = '\0';
  }
  ImGui::Checkbox("States", &filterStates);
  ImGui::SameLine();
  ImGui::Checkbox("Layers", &filterLayers);

  ImGui::Separator();

  if (collapsed not_eq -1) {
    ImGui::SetNextItemOpen(collapsed not_eq 0);
  }

  if (states_manager->getCurrentState() == nullptr) {
    ImGui::Text("No states");
  }

  for (const auto& [state, layers] : states_manager->getMetrics()) {
    if (filterStates and states_text_filter[0] not_eq '\0' and
        strstr(state.c_str(), states_text_filter) == nullptr) {
      continue;
    }
    if (ImGui::CollapsingHeader(state.c_str())) {
      std::map<std::string, std::string, NumericComparator> layersMap =
        JsonToMap(layers);
      for (const auto& [name, enabled] : layersMap) {
        if (filterLayers and states_text_filter[0] not_eq '\0' and
            strstr(name.c_str(), states_text_filter) == nullptr) {
          continue;
        }
        ImGui::BulletText("%s - %s", name.c_str(), enabled.c_str());
      }
    }
  }
}
}

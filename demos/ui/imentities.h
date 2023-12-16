#pragma once

#include <entt/entt.hpp>

#include "engineAPI.h"

namespace demos {

std::string selectedEntityTabKey;
char textFilterEntities[128]{}; // TODO: move to class
bool filterPrefabs{false};
bool filterPrototypes{false};
bool filterInstances{false};

void drawEntities(engine::SceneManager& sm) {
  entt::registry& registry = sm.getRegistry();

  if (registry.empty()) {
    ImGui::Text("No entities loaded");
    return;
  }

  int collapsed = engine::ui::collapser();

  ImGui::InputText("##filter", textFilterEntities, IM_ARRAYSIZE(textFilterEntities));
  if (ImGui::IsItemHovered()) {
    ImGui::SetTooltip("Filter entities by name");
  }
  ImGui::SameLine();
  if (ImGui::Button("Clear Filter")) {
    textFilterEntities[0] = '\0';
  }
  ImGui::Checkbox("Prefabs", &filterPrefabs);
  ImGui::SameLine();
  ImGui::Checkbox("Prototypes", &filterPrototypes);
  ImGui::SameLine();
  ImGui::Checkbox("Instances", &filterInstances);

  ImGui::Separator();
  ImGui::Columns(2);

  if (collapsed not_eq -1)
    ImGui::SetNextItemOpen(collapsed not_eq 0);

  if (ImGui::CollapsingHeader("Prefabs")) {
    for (const auto& [prefabID, prototypes] : sm.getAllPrototypes()) {
      std::string prefabName = "Prefab " + prefabID;
      if (filterPrefabs and textFilterEntities[0] not_eq '\0' and strstr(prefabName.c_str(), textFilterEntities) == nullptr) {
        continue;
      }
      if (ImGui::TreeNode(prefabName.c_str())) {
        for (auto& [prototypeID, entity] : prototypes) {
          if (filterPrototypes and textFilterEntities[0] not_eq '\0' and strstr(prototypeID.c_str(), textFilterEntities) == nullptr) {
            continue;
          }
          if (ImGui::Selectable(prototypeID.c_str())) {
            selectedEntityTabKey = std::to_string(entt::to_integral(entity));
          }
        }
        ImGui::TreePop();
      }
    }
  }

  if (collapsed not_eq -1)
    ImGui::SetNextItemOpen(collapsed not_eq 0);

  if (ImGui::CollapsingHeader("Instances")) {
    for (const auto& [name, entity] : sm.getAllNamedEntities()) {
      if (filterInstances and textFilterEntities[0] not_eq '\0' and strstr(name.c_str(), textFilterEntities) == nullptr) {
        continue;
      }
      if (ImGui::Selectable(name.c_str())) {
        selectedEntityTabKey = std::to_string(entt::to_integral(entity));
      }
    }
  }

  if (collapsed == 0) {
    selectedEntityTabKey = "";
  }

  ImGui::NextColumn();
  if (not selectedEntityTabKey.empty()) {
    entt::entity entity = entt::entity(std::stoul(selectedEntityTabKey));
    if (registry.valid(entity)) {
      ImGui::Text("Components");
    }
  }

  ImGui::Columns(1);
}
}
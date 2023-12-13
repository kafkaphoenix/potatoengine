#pragma once

#include <entt/entt.hpp>

#include "engineAPI.h"

namespace demos {

std::string selectedEntityTabKey;

void drawEntities(engine::SceneManager& sm) {
  entt::registry& registry = sm.getRegistry();

  if (registry.empty()) {
    ImGui::Text("No entities loaded");
    return;
  }

  int collapsed = engine::ui::collapser();

  ImGui::Columns(2);

  if (collapsed != -1)
    ImGui::SetNextItemOpen(collapsed != 0);

  if (ImGui::CollapsingHeader("Prototypes")) {
    for (const auto& [prefabID, prototypes] : sm.getAllPrototypes()) {
      std::string prefabName = "Prefab " + prefabID;
      if (ImGui::TreeNode(prefabName.c_str())) {
        for (auto& [prototypeID, entity] : prototypes) {
          if (ImGui::Selectable(prototypeID.c_str())) {
            selectedEntityTabKey = std::to_string(entt::to_integral(entity));
          }
        }
        ImGui::TreePop();
      }
    }
  }

  if (collapsed != -1)
    ImGui::SetNextItemOpen(collapsed != 0);

  if (ImGui::CollapsingHeader("Instances")) {
    for (const auto& [name, entity] : sm.getAllNamedEntities()) {
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
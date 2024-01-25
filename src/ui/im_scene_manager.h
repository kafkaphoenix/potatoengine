#pragma once

#include <entt/entt.hpp>
#include <imgui.h>

#include "pch.h"
#include "scene/sceneManager.h"
#include "settings.h"
#include "ui/imutils.h"
#include "utils/mapJsonSerializer.h"
#include "utils/numericComparator.h"

using namespace entt::literals;

namespace potatoengine::ui {

std::string selectedSceneManagerTabKey;
char scene_objects_text_filter[128]{}; // TODO: move to class
bool filterPrefabs{};
bool filterPrototypes{};
bool filterInstances{};
bool filterComponents{};
bool filterSystems{};

void drawChildInfo(
  const std::map<std::string, std::string, NumericComparator>& infoData,
  const std::string& sceneName) {
  for (const auto& [key, value] : infoData) {
    if (key.starts_with("mesh ") or key.starts_with("texture ") or
        key.starts_with("material ") or key.starts_with("transform ") or
        (key.starts_with("vao ") and value != "undefined")) {
      if (ImGui::TreeNode((key + sceneName).c_str(), key.c_str())) {
        auto childInfoData = JsonToMap(value);
        for (const auto& [key, value] : childInfoData) {
          if (key.starts_with("texture ") or
              (key.starts_with("vao ") and value != "undefined")) {
            if (ImGui::TreeNode((key + sceneName).c_str(), key.c_str())) {
              // CBody, CShape, CChunk have a CMesh that has a VAO and CTexture
              auto rechildInfoData = JsonToMap(value);
              for (const auto& [key, value] : rechildInfoData) {
                ImGui::BulletText("%s: %s", key.c_str(), value.c_str());
              }
              ImGui::TreePop();
            }
          } else {
            ImGui::BulletText("%s: %s", key.c_str(), value.c_str());
          }
        }
        ImGui::TreePop();
      }
    } else {
      ImGui::BulletText("%s: %s", key.c_str(), value.c_str());
    }
  }
}

void drawSceneManager(const std::unique_ptr<SceneManager>& scene_manager,
                      const std::unique_ptr<Settings>& settings) {
  entt::registry& registry = scene_manager->getRegistry();

  if (registry.storage<entt::entity>().in_use() == 0) {
    ImGui::Text("No entities loaded");
    return;
  }

  int collapsed = collapser();

  ImGui::InputText("##filter", scene_objects_text_filter,
                   IM_ARRAYSIZE(scene_objects_text_filter));
  if (ImGui::IsItemHovered()) {
    ImGui::SetTooltip("Filter entities or components by name");
  }
  ImGui::SameLine();
  if (ImGui::Button("Clear Filter")) {
    scene_objects_text_filter[0] = '\0';
  }
  ImGui::Checkbox("Prefabs", &filterPrefabs);
  ImGui::SameLine();
  ImGui::Checkbox("Prototypes", &filterPrototypes);
  ImGui::SameLine();
  ImGui::Checkbox("Instances", &filterInstances);
  ImGui::SameLine();
  ImGui::Checkbox("Components", &filterComponents);
  ImGui::SameLine();
  ImGui::Checkbox("Systems", &filterSystems);

  ImGui::Separator();
  ImGui::Columns(2);

  if (collapsed not_eq -1) {
    ImGui::SetNextItemOpen(collapsed not_eq 0);
  }

  if (ImGui::CollapsingHeader("Prefabs")) {
    for (const auto& [prefabID, prototypes] :
         scene_manager->getAllPrototypes()) {
      std::string prefabName = "Prefab " + prefabID;
      if (filterPrefabs and scene_objects_text_filter[0] not_eq '\0' and
          strstr(prefabName.c_str(), scene_objects_text_filter) == nullptr) {
        continue;
      }
      if (ImGui::TreeNode((prefabName + settings->activeScene).c_str(),
                          prefabName.c_str())) {
        for (auto& [prototypeID, entity] : prototypes) {
          if (filterPrototypes and scene_objects_text_filter[0] not_eq '\0' and
              strstr(prototypeID.c_str(), scene_objects_text_filter) ==
                nullptr) {
            continue;
          }
          if (ImGui::Selectable(prototypeID.c_str())) {
            selectedSceneManagerTabKey =
              std::to_string(entt::to_integral(entity));
          }
        }
        ImGui::TreePop();
      }
    }
  }

  if (collapsed not_eq -1) {
    ImGui::SetNextItemOpen(collapsed not_eq 0);
  }

  if (ImGui::CollapsingHeader("Instances")) {
    for (const auto& [name, entity] : scene_manager->getNamedEntities()) {
      if (filterInstances and scene_objects_text_filter[0] not_eq '\0' and
          strstr(name.c_str(), scene_objects_text_filter) == nullptr) {
        continue;
      }
      if (ImGui::Selectable(name.c_str())) {
        selectedSceneManagerTabKey = std::to_string(entt::to_integral(entity));
      }
    }
  }

  if (collapsed not_eq -1) {
    ImGui::SetNextItemOpen(collapsed not_eq 0);
  }

  if (ImGui::CollapsingHeader("Systems")) {
    for (const auto& name : scene_manager->getNamedSystems()) {
      if (filterSystems and scene_objects_text_filter[0] not_eq '\0' and
          strstr(name.c_str(), scene_objects_text_filter) == nullptr) {
        continue;
      }
      ImGui::Text("%s", name.c_str());
    }
  }

  if (collapsed == 0 or settings->reloadScene) {
    selectedSceneManagerTabKey.clear();
  }

  ImGui::NextColumn();
  if (not selectedSceneManagerTabKey.empty()) {
    entt::entity entity = entt::entity(std::stoul(selectedSceneManagerTabKey));
    if (registry.valid(entity)) {
      ImGui::SeparatorText("Components");
      std::string cName;
      entt::meta_type cType;
      entt::meta_any cData;
      entt::meta_func getInfoFunc;
      entt::meta_any info;
      std::map<std::string, std::string, NumericComparator> infoData;
      for (auto&& [id, storage] : registry.storage()) {
        if (storage.contains(entity)) {
          cName = storage.type().name();
          cName = cName.substr(cName.find_last_of(':') + 1);
          if (filterComponents and scene_objects_text_filter[0] not_eq '\0' and
              strstr(cName.c_str(), scene_objects_text_filter) == nullptr) {
            continue;
          }
          cType = entt::resolve(storage.type());
          cData = cType.construct(storage.value(entity));
          getInfoFunc = cType.func("getInfo"_hs);
          if (getInfoFunc) {
            if (ImGui::TreeNode(
                  (selectedSceneManagerTabKey + cName + settings->activeScene)
                    .c_str(),
                  cName.c_str())) {
              info = getInfoFunc.invoke(cData);
              if (info) {
                infoData = info.cast<
                  std::map<std::string, std::string, NumericComparator>>();
                if (infoData.empty()) {
                  ImGui::Text("no data");
                } else {
                  drawChildInfo(infoData, settings->activeScene);
                }
              } else {
                ENGINE_ERROR("Failed to get info for component {0}", cName);
                ImGui::Text("No info method defined");
              }
              ImGui::TreePop();
            }
          } else {
            if (ImGui::TreeNode(
                  (selectedSceneManagerTabKey + cName + settings->activeScene)
                    .c_str(),
                  cName.c_str())) {
              ENGINE_ERROR("Failed to get info for component {0}", cName);
              ImGui::Text("No info method defined");
              ImGui::TreePop();
            }
          }
        }
      }
    }
  }
  ImGui::Columns(1);
}
}
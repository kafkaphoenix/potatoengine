#pragma once

#include <imgui.h>

#include "pch.h"
#include "renderer/renderer.h"
#include "settings.h"
#include "ui/imutils.h"
#include "utils/mapJsonSerializer.h"

namespace potatoengine::ui {

std::string selectedRenderManagerTabKey;
std::string selectedRenderManagerTabType;
char render_objects_text_filter[128]{}; // TODO: move to class
bool filterFBOS{};
bool filterShaderPrograms{};
bool filterShaderInfo{};

void drawRenderManager(const std::unique_ptr<Renderer>& renderer,
                       const std::unique_ptr<Settings>& settings) {
  const auto& fbos = renderer->getFramebuffers();
  const auto& sp = renderer->getShaderPrograms();

  int collapsed = collapser();

  ImGui::InputText("##filter", render_objects_text_filter,
                   IM_ARRAYSIZE(render_objects_text_filter));
  if (ImGui::IsItemHovered()) {
    ImGui::SetTooltip("Filter renderer objects by name");
  }
  ImGui::SameLine();
  if (ImGui::Button("Clear Filter")) {
    render_objects_text_filter[0] = '\0';
  }
  ImGui::Checkbox("FBOS", &filterFBOS);
  ImGui::SameLine();
  ImGui::Checkbox("Shader programs", &filterShaderPrograms);
  ImGui::SameLine();
  ImGui::Checkbox("Shader info", &filterShaderInfo);

  ImGui::Separator();
  ImGui::Columns(2);

  if (collapsed not_eq -1) {
    ImGui::SetNextItemOpen(collapsed not_eq 0);
  }

  if (ImGui::CollapsingHeader("FBOS")) {
    if (fbos.empty()) {
      ImGui::Text("No FBOS");
    }
    for (const auto& [key, value] : fbos) {
      if (filterFBOS and render_objects_text_filter[0] not_eq '\0' and
          strstr(key.c_str(), render_objects_text_filter) == nullptr) {
        continue;
      }
      if (ImGui::Selectable(key.c_str())) {
        selectedRenderManagerTabKey = key;
        selectedRenderManagerTabType = "FBO";
      }
    }
  }

  if (collapsed not_eq -1) {
    ImGui::SetNextItemOpen(collapsed not_eq 0);
  }

  if (ImGui::CollapsingHeader("Shader programs")) {
    if (sp.empty()) {
      ImGui::Text("No Shader programs");
    }
    for (const auto& [key, value] : sp) {
      if (filterShaderPrograms and render_objects_text_filter[0] not_eq '\0' and
          strstr(key.c_str(), render_objects_text_filter) == nullptr) {
        continue;
      }
      if (ImGui::Selectable(key.c_str())) {
        selectedRenderManagerTabKey = key;
        selectedRenderManagerTabType = "Shader Program";
      }
    }
  }

  if (collapsed == 0 or settings->reloadScene) {
    selectedRenderManagerTabKey.clear();
    selectedRenderManagerTabType.clear();
  }

  ImGui::NextColumn();
  if (not selectedRenderManagerTabKey.empty()) {
    if (selectedRenderManagerTabType == "Shader Program") {
      const auto& shaderProgram = sp.at(selectedRenderManagerTabKey);
      const auto& shaderProgramInfo = shaderProgram->getInfo();
      for (const auto& [key, value] : shaderProgramInfo) {
        if (filterShaderInfo and render_objects_text_filter[0] not_eq '\0' and
            strstr(key.c_str(), render_objects_text_filter) == nullptr) {
          continue;
        }
        ImGui::BulletText("%s: %s", key.c_str(), value.c_str());
      }
    } else if (selectedRenderManagerTabType == "FBO") {
      const auto& value = fbos.at(selectedRenderManagerTabKey);
      const auto& fboInfo = value->getInfo();
      for (const auto& [key, value] : fboInfo) {
        if (key == "Color texture" or key == "Depth texture") {
          const auto& textureInfo = JsonToMap(value);
          if (ImGui::TreeNode((selectedRenderManagerTabType +
                               selectedRenderManagerTabKey + key +
                               settings->activeScene)
                                .c_str(),
                              key.c_str())) {
            for (const auto& [key, value] : textureInfo) {
              ImGui::BulletText("%s: %s", key.c_str(), value.c_str());
            }
            ImGui::TreePop();
          }
        } else {
          ImGui::BulletText("%s: %s", key.c_str(), value.c_str());
        }
      }
    }
  }

  ImGui::Columns(1);
}
}

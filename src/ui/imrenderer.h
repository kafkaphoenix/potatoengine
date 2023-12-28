#pragma once

#include <imgui.h>

#include "pch.h"
#include "renderer/renderer.h"
#include "settings.h"
#include "ui/imutils.h"
#include "utils/mapJsonSerializer.h"

namespace potatoengine::ui {

std::string selectedRendererTabKey;
std::string selectedRendererTabType;
char textFilterRenderer[128]{}; // TODO: move to class
bool filterFBOS{};
bool filterShaderPrograms{};
bool filterShaderInfo{};

void drawRenderer(std::weak_ptr<Renderer> r, std::weak_ptr<Settings> s) {
  const auto& renderer = r.lock();
  ENGINE_ASSERT(renderer, "Renderer is null!");

  const auto& settings = s.lock();
  ENGINE_ASSERT(settings, "Settings is null!");

  const auto& fbos = renderer->getFramebuffers();
  const auto& sp = renderer->getShaderPrograms();

  int collapsed = collapser();

  ImGui::InputText("##filter", textFilterRenderer,
                   IM_ARRAYSIZE(textFilterRenderer));
  if (ImGui::IsItemHovered()) {
    ImGui::SetTooltip("Filter renderer objects by name");
  }
  ImGui::SameLine();
  if (ImGui::Button("Clear Filter")) {
    textFilterRenderer[0] = '\0';
  }
  ImGui::Checkbox("FBOS", &filterFBOS);
  ImGui::SameLine();
  ImGui::Checkbox("Shader Programs", &filterShaderPrograms);
  ImGui::SameLine();
  ImGui::Checkbox("Shader Info", &filterShaderInfo);

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
      if (filterFBOS and textFilterRenderer[0] not_eq '\0' and
          strstr(key.c_str(), textFilterRenderer) == nullptr) {
        continue;
      }
      if (ImGui::Selectable(key.c_str())) {
        selectedRendererTabKey = key;
        selectedRendererTabType = "FBO";
      }
    }
  }

  if (collapsed not_eq -1) {
    ImGui::SetNextItemOpen(collapsed not_eq 0);
  }

  if (ImGui::CollapsingHeader("Shader Programs")) {
    if (sp.empty()) {
      ImGui::Text("No Shader Programs");
    }
    for (const auto& [key, value] : sp) {
      if (filterShaderPrograms and textFilterRenderer[0] not_eq '\0' and
          strstr(key.c_str(), textFilterRenderer) == nullptr) {
        continue;
      }
      if (ImGui::Selectable(key.c_str())) {
        selectedRendererTabKey = key;
        selectedRendererTabType = "Shader Program";
      }
    }
  }

  if (collapsed == 0 or settings->reloadScene) {
    selectedRendererTabKey.clear();
    selectedRendererTabType.clear();
  }

  ImGui::NextColumn();
  if (not selectedRendererTabKey.empty()) {
    if (selectedRendererTabType == "Shader Program") {
      const auto& shaderProgram = sp.at(selectedRendererTabKey);
      const auto& shaderProgramInfo = shaderProgram->getInfo();
      for (const auto& [key, value] : shaderProgramInfo) {
        if (filterShaderInfo and textFilterRenderer[0] not_eq '\0' and
            strstr(key.c_str(), textFilterRenderer) == nullptr) {
          continue;
        }
        ImGui::BulletText("%s: %s", key.c_str(), value.c_str());
      }
    } else if (selectedRendererTabType == "FBO") {
      const auto& value = fbos.at(selectedRendererTabKey);
      const auto& fboInfo = value->getInfo();
      for (const auto& [key, value] : fboInfo) {
        if (key == "Color texture" or key == "Depth texture") {
          const auto& textureInfo = JsonToMap(value);
          if (ImGui::TreeNode(
                (selectedRendererTabType + selectedRendererTabKey + key + settings->activeScene)
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

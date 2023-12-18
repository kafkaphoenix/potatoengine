#pragma once

#include "engineAPI.h"

namespace demos {

std::string selectedRendererTabKey;
std::string selectedRendererTabType;
char textFilterRenderer[128]{}; // TODO: move to class
bool filterFBOS{};
bool filterShaderPrograms{};

void drawRenderer(std::weak_ptr<engine::Renderer> r) {
  auto renderer = r.lock();
  APP_ASSERT(renderer, "Renderer is null!");

  const auto& fbos = renderer->getFramebuffers();
  const auto& sp = renderer->getShaderPrograms();

  int collapsed = engine::ui::collapser();

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

  ImGui::Separator();
  ImGui::Columns(2);

  if (collapsed not_eq -1)
    ImGui::SetNextItemOpen(collapsed not_eq 0);

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

  if (collapsed not_eq -1)
    ImGui::SetNextItemOpen(collapsed not_eq 0);

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

  if (collapsed == 0) {
    selectedRendererTabKey = "";
    selectedRendererTabType = "";
  }

  ImGui::NextColumn();
  if (not selectedRendererTabKey.empty()) {
    if (selectedRendererTabType == "Shader Program") {
      const auto& shaderProgram = sp.at(selectedRendererTabKey);
      const auto& shaderProgramInfo = shaderProgram->getInfo();
      for (const auto& [key, value] : shaderProgramInfo) {
        ImGui::BulletText("%s: %s", key.c_str(), value.c_str());
      }
    } else if (selectedRendererTabType == "FBO") {
      const auto& value = fbos.at(selectedRendererTabKey);
      const auto& fboInfo = value->getInfo();
      for (const auto& [key, value] : fboInfo) {
        ImGui::BulletText("%s: %s", key.c_str(), value.c_str());
      }
      const auto& colorTexture = value->getColorTexture();
      if (colorTexture) {
        const auto& textureInfo = colorTexture->getInfo();
        if (ImGui::TreeNode((selectedRendererTabType + selectedRendererTabKey + "Color Texture").c_str(), "Color Texture")) {
          for (const auto& [key, value] : textureInfo) {
            ImGui::BulletText("%s: %s", key.c_str(), value.c_str());
          }
          ImGui::TreePop();
        }
      } else {
        ImGui::BulletText("Color Texture: nullptr");
      }
      const auto& depthTexture = value->getDepthTexture();
      if (depthTexture) {
        const auto& textureInfo = depthTexture->getInfo();
        if (ImGui::TreeNode((selectedRendererTabType + selectedRendererTabKey + "Depth Texture").c_str(), "Depth Texture")) {
          for (const auto& [key, value] : textureInfo) {
            ImGui::BulletText("%s: %s", key.c_str(), value.c_str());
          }
          ImGui::TreePop();
        }
      } else {
        ImGui::BulletText("Depth Texture: nullptr");
      }
    }
  }

  ImGui::Columns(1);
}
}

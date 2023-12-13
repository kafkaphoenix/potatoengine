#pragma once

#include "engineAPI.h"

namespace demos {

void drawMetrics(bool* show_tool_metrics, std::weak_ptr<engine::AssetsManager> am, std::weak_ptr<engine::Renderer> r,
                 engine::SceneManager& sm) {
  if (not *show_tool_metrics)
    return;
  const auto& manager = am.lock();
  APP_ASSERT(manager, "AssetsManager is null!");
  const auto& renderer = r.lock();
  APP_ASSERT(renderer, "Renderer is null!");

  ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
  if (ImGui::Begin("Metrics", show_tool_metrics)) {
    ImGui::SeparatorText("App");
    ImGui::Text("FPS: %d", static_cast<int>(ImGui::GetIO().Framerate));
    ImGui::SameLine();
    engine::ui::underline(ImColor(255, 255, 255));
    engine::ui::helpMark("Frames per second");

    ImGui::SeparatorText("Scene Manager");
    for (const auto& [key, value] : sm.getMetrics()) {
      ImGui::Text("%s: %s", key.c_str(), value.c_str());
    }

    ImGui::SeparatorText("Asset Manager");
    for (const auto& [key, value] : manager->getMetrics()) {
      ImGui::Text("%s: %s", key.c_str(), value.c_str());
    }

    ImGui::SeparatorText("Renderer");
    ImGui::Text("Framebuffers: %d", renderer->getFramebuffers().size());
    ImGui::Text("Shader programs: %d", renderer->getShaderPrograms().size());

    ImGui::SeparatorText("Hardware");
    ImGui::Text("GPU: %s", glGetString(GL_RENDERER));
    ImGui::Text("OpenGL version: %s", glGetString(GL_VERSION));
    ImGui::Text("GLSL version: %s", glGetString(GL_SHADING_LANGUAGE_VERSION));
  }
  ImGui::End();
}
}
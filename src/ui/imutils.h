#pragma once

#include <imgui.h>

#include "pch.h"

namespace potatoengine::ui {
inline void helpMark(std::string_view helpText) {
  ImGui::TextDisabled("(?)");
  if (ImGui::IsItemHovered()) {
    if (ImGui::BeginTooltip()) {
      ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.f);
      ImGui::TextUnformatted(helpText.data());
      ImGui::PopTextWrapPos();
    }
    ImGui::EndTooltip();
  }
}

inline void underline(ImColor color, float thickness = 1.f) {
  ImVec2 min = ImGui::GetItemRectMin();
  ImVec2 max = ImGui::GetItemRectMax();
  min.y = max.y;
  ImGui::GetWindowDrawList()->AddLine(min, max, color, thickness);
}

inline void url(std::string_view url_name, std::string_view url_link) {
  ImGui::TextColored(ImColor(0, 255, 255), "%s", url_name.data());
  if (ImGui::IsItemHovered()) {
    if (ImGui::IsItemClicked()) {
      system(std::format("start {}", url_link.data()).data());
    }
    underline(ImColor(0, 255, 255));
  } else {
    underline(ImColor(255, 255, 255));
  }
}

inline int collapser() {
  int collapsed = -1;

  auto button_color = ImGui::GetStyleColorVec4(ImGuiCol_Button);
  ImGui::PushStyleColor(ImGuiCol_Border, button_color);
  ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.f);
  button_color.w = 0.f;
  ImGui::PushStyleColor(ImGuiCol_Button, button_color);

  if (ImGui::Button("Expand all"))
    collapsed = 1;
  ImGui::SameLine();
  if (ImGui::Button("Collapse all"))
    collapsed = 0;

  ImGui::PopStyleColor(2);
  ImGui::PopStyleVar();

  return collapsed;
}
}
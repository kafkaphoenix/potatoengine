#pragma once

#include <imgui.h>

#include "potatoengine/pch.h"

namespace potatoengine::ui {
inline void helpMark(std::string_view helpText) {
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered()) {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(helpText.data());
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

inline void underline(ImColor color, float thickness = 1.0f) {
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
    if (ImGui::Button("Expand all"))
        collapsed = 1;
    ImGui::SameLine();
    if (ImGui::Button("Collapse all"))
        collapsed = 0;

    return collapsed;
}
}
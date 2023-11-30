#pragma once

#include "potatoengine/engineAPI.h"

namespace demos {

std::string selectedRendererTabKey;

void drawRenderer(std::weak_ptr<engine::Renderer> r) {
    auto renderer = r.lock();
    APP_ASSERT(renderer, "Renderer is null!");

    const auto& fbos = renderer->getFramebuffers();
    if (fbos.empty()) { // TODO after other renderer objects are added, this should be a check for all renderer objects
        ImGui::Text("No FBOS");
        return;
    }

    int collapsed = engine::ui::collapser();

    ImGui::Columns(2);

    if (collapsed != -1)
        ImGui::SetNextItemOpen(collapsed != 0);

    if (ImGui::CollapsingHeader("FBOS")) {
        for (const auto& [key, value] : fbos) {
            if (ImGui::Selectable(key.c_str())) {
                selectedRendererTabKey = key;
            }
        }
    }

    // TODO: other renderer objects (shaders, textures, vao, vbo, etc)

    if (collapsed == 0) {
        selectedRendererTabKey = "";
    }

    ImGui::NextColumn();
    if (not selectedRendererTabKey.empty()) {
        const auto& value = fbos.at(selectedRendererTabKey);
        const auto& fboInfo = value->getInfo();
        for (const auto& [key, value] : fboInfo) {
            ImGui::BulletText("%s: %s", key.c_str(), value.c_str());
        }
        const auto& colorTexture = value->getColorTexture();
        if (colorTexture) {
            const auto& textureInfo = colorTexture->getInfo();
            if (ImGui::TreeNode("Color Texture")) {
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
            if (ImGui::TreeNode("Depth Texture")) {
                for (const auto& [key, value] : textureInfo) {
                    ImGui::BulletText("%s: %s", key.c_str(), value.c_str());
                }
                ImGui::TreePop();
            }
        } else {
            ImGui::BulletText("Depth Texture: nullptr");
        }
    }

    ImGui::Columns(1);
}
}

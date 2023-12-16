#pragma once

#include "engineAPI.h"
#include "settings.h" // linter

namespace demos {

std::string selectedSettingTabKey;

void drawSettings(Settings& settings) {
  ImGui::Columns(2);

  ImGui::SeparatorText("Engine");
  if (ImGui::Selectable("Window")) {
    selectedSettingTabKey = "Window";
  }
  if (ImGui::Selectable("Cursor")) {
    selectedSettingTabKey = "Cursor";
  }
  if (ImGui::Selectable("Debug")) {
    selectedSettingTabKey = "Debug";
  }
  ImGui::SeparatorText("App");
  if (ImGui::Selectable("Renderer")) {
    selectedSettingTabKey = "Renderer";
  }
  if (ImGui::Selectable("Scene")) {
    selectedSettingTabKey = "Scene";
  }

  ImGui::NextColumn();
  if (not selectedSettingTabKey.empty()) {
    ImGui::SeparatorText("Edit");
    if (selectedSettingTabKey == "Window") {
      ImGui::InputText("Window icon path", &settings.windowIconPath);
      ImGui::InputInt("Window width", &settings.windowWidth);
      ImGui::InputInt("Window height", &settings.windowHeight);
      ImGui::InputInt("Depth bits", &settings.depthBits);
      ImGui::InputInt("Refresh rate", &settings.refreshRate);
      ImGui::Checkbox("Fullscreen", &settings.fullscreen);
      ImGui::InputInt("Primary monitor", &settings.primaryMonitor);
      ImGui::Checkbox("VSync", &settings.vSync);
      ImGui::Checkbox("Resizable", &settings.resizable);
      ImGui::InputInt("OpenGL major version", &settings.openglMajorVersion);
      ImGui::InputInt("OpenGL minor version", &settings.openglMinorVersion);
    } else if (selectedSettingTabKey == "Cursor") {
      ImGui::InputText("Cursor icon path", &settings.cursorIconPath);
      if (ImGui::BeginCombo("Cursor mode", settings.cursorMode == 0   ? "Normal"
                                           : settings.cursorMode == 1 ? "Hidden"
                                           : settings.cursorMode == 2
                                             ? "Disabled"
                                             : "Unknown")) {
        for (int n = 0; n < settings.cursorModes.size(); n++) {
          bool is_selected = (settings.cursorMode == n);
          if (ImGui::Selectable(settings.cursorModes[n], is_selected)) {
            if (settings.cursorMode not_eq n) {
              settings.cursorMode = n;
            }
          }
          if (is_selected) {
            ImGui::SetItemDefaultFocus();
          }
        }
        ImGui::EndCombo();
      }
    } else if (selectedSettingTabKey == "Debug") {
      ImGui::Checkbox("Debug enabled", &settings.debugEnabled);
      if (ImGui::BeginCombo("Debug level", settings.debugLevel == 0   ? "Trace"
                                           : settings.debugLevel == 1 ? "Debug"
                                           : settings.debugLevel == 2 ? "Info"
                                           : settings.debugLevel == 3 ? "Warn"
                                           : settings.debugLevel == 4 ? "Error"
                                           : settings.debugLevel == 5
                                             ? "Critical"
                                             : "Unknown")) {
        for (int n = 0; n < settings.debugLevels.size(); n++) {
          bool is_selected = (settings.debugLevel == n);
          if (ImGui::Selectable(settings.debugLevels[n], is_selected)) {
            if (settings.debugLevel not_eq n) {
              settings.debugLevel = n;
            }
          }
          if (is_selected) {
            ImGui::SetItemDefaultFocus();
          }
        }
        ImGui::EndCombo();
      }
      ImGui::Checkbox("Display FPS", &settings.displayFPS);
    } else if (selectedSettingTabKey == "Renderer") {
      ImGui::ColorEdit4("Clear color", settings.clearColor.data());
      ImGui::SliderFloat("Clear depth", &settings.clearDepth, 0.f, 1.f);
    } else if (selectedSettingTabKey == "Scene") {
      ImGui::Text("Name: %s", settings.activeScene.c_str());
      ImGui::Text("Filepath: %s ", settings.activeScenePath.c_str());
      if (ImGui::BeginCombo("Active scene", settings.activeScene.c_str())) {
        for (int n = 0; n < settings.scenes.size(); n++) {
          bool is_selected = (settings.activeScene == settings.scenes[n]);
          if (ImGui::Selectable(settings.scenes[n], is_selected)) {
            if (settings.activeScene not_eq settings.scenes[n]) {
              settings.reloadScene = true;
              settings.activeScene = settings.scenes[n];
              settings.activeScenePath = settings.scenesPaths[n];
            }
          }
          if (is_selected) {
            ImGui::SetItemDefaultFocus();
          }
        }
        ImGui::EndCombo();
      }
      ImGui::Checkbox("Reload scene", &settings.reloadScene);
    }
  }

  ImGui::Columns(1);
}
}
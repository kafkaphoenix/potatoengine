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
  if (ImGui::Selectable("Logger")) {
    selectedSettingTabKey = "Logger";
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
      ImGui::Checkbox("Display FPS", &settings.displayFPS);
    } else if (selectedSettingTabKey == "Logger") {
      ImGui::Checkbox("Enable engine logger", &settings.enableEngineLogger);
      if (settings.enableEngineLogger and not engine::LogManager::isEngineLoggerEnabled()) {
        engine::LogManager::toggleEngineLogger(true);
      }
      if (not settings.enableEngineLogger and engine::LogManager::isEngineLoggerEnabled()) {
        engine::LogManager::toggleEngineLogger(false);
      }
      if (ImGui::BeginCombo("Engine log level",
                            settings.engineLogLevel == 0   ? "Trace"
                            : settings.engineLogLevel == 1 ? "Debug"
                            : settings.engineLogLevel == 2 ? "Info"
                            : settings.engineLogLevel == 3 ? "Warning"
                            : settings.engineLogLevel == 4 ? "Error"
                            : settings.engineLogLevel == 5 ? "Critical"
                                                           : "Unknown")) {
        for (int n = 0; n < settings.logLevels.size(); n++) {
          bool is_selected = (settings.engineLogLevel == n);
          if (ImGui::Selectable(settings.logLevels[n], is_selected)) {
            if (settings.engineLogLevel not_eq n) {
              settings.engineLogLevel = n;
              engine::LogManager::SetEngineLoggerLevel(
                static_cast<spdlog::level::level_enum>(n));
            }
          }
          if (is_selected) {
            ImGui::SetItemDefaultFocus();
          }
        }
        ImGui::EndCombo();
      }
      if (ImGui::BeginCombo("Engine log flush level",
                            settings.engineFlushLevel == 0   ? "Trace"
                            : settings.engineFlushLevel == 1 ? "Debug"
                            : settings.engineFlushLevel == 2 ? "Info"
                            : settings.engineFlushLevel == 3 ? "Warning"
                            : settings.engineFlushLevel == 4 ? "Error"
                            : settings.engineFlushLevel == 5 ? "Critical"
                                                             : "Unknown")) {
        for (int n = 0; n < settings.logLevels.size(); n++) {
          bool is_selected = (settings.engineFlushLevel == n);
          if (ImGui::Selectable(settings.logLevels[n], is_selected)) {
            if (settings.engineFlushLevel not_eq n) {
              settings.engineFlushLevel = n;
              engine::LogManager::SetEngineLoggerFlushLevel(
                static_cast<spdlog::level::level_enum>(n));
            }
          }
          if (is_selected) {
            ImGui::SetItemDefaultFocus();
          }
        }
        ImGui::EndCombo();
      }
      ImGui::Checkbox("Enable app logger", &settings.enableAppLogger);
      if (settings.enableAppLogger and not engine::LogManager::isAppLoggerEnabled()) {
        engine::LogManager::toggleAppLogger(true);
      }
      if (not settings.enableAppLogger and engine::LogManager::isAppLoggerEnabled()) {
        engine::LogManager::toggleAppLogger(false);
      }
      if (ImGui::BeginCombo("App log level",
                            settings.appLogLevel == 0   ? "Trace"
                            : settings.appLogLevel == 1 ? "Debug"
                            : settings.appLogLevel == 2 ? "Info"
                            : settings.appLogLevel == 3 ? "Warning"
                            : settings.appLogLevel == 4 ? "Error"
                            : settings.appLogLevel == 5 ? "Critical"
                                                        : "Unknown")) {
        for (int n = 0; n < settings.logLevels.size(); n++) {
          bool is_selected = (settings.appLogLevel == n);
          if (ImGui::Selectable(settings.logLevels[n], is_selected)) {
            if (settings.appLogLevel not_eq n) {
              settings.appLogLevel = n;
              engine::LogManager::SetAppLoggerLevel(
                static_cast<spdlog::level::level_enum>(n));
            }
          }
          if (is_selected) {
            ImGui::SetItemDefaultFocus();
          }
        }
        ImGui::EndCombo();
      }
      if (ImGui::BeginCombo("App log flush level",
                            settings.appFlushLevel == 0   ? "Trace"
                            : settings.appFlushLevel == 1 ? "Debug"
                            : settings.appFlushLevel == 2 ? "Info"
                            : settings.appFlushLevel == 3 ? "Warning"
                            : settings.appFlushLevel == 4 ? "Error"
                            : settings.appFlushLevel == 5 ? "Critical"
                                                          : "Unknown")) {
        for (int n = 0; n < settings.logLevels.size(); n++) {
          bool is_selected = (settings.appFlushLevel == n);
          if (ImGui::Selectable(settings.logLevels[n], is_selected)) {
            if (settings.appFlushLevel not_eq n) {
              settings.appFlushLevel = n;
              engine::LogManager::SetAppLoggerFlushLevel(
                static_cast<spdlog::level::level_enum>(n));
            }
          }
          if (is_selected) {
            ImGui::SetItemDefaultFocus();
          }
        }
        ImGui::EndCombo();
      }
      ImGui::Checkbox("Enable engine backtrace logger",
                      &settings.enableEngineBacktraceLogger);
      if (settings.enableEngineBacktraceLogger and not engine::LogManager::isEngineBacktraceLoggerEnabled()) {
        engine::LogManager::toggleAppBacktraceLogger(true);
      }
      if (not settings.enableEngineBacktraceLogger and engine::LogManager::isEngineBacktraceLoggerEnabled()) {
        engine::LogManager::toggleEngineBacktraceLogger(false);
      }
      ImGui::Checkbox("Enable app backtrace logger",
                      &settings.enableAppBacktraceLogger);
      if (settings.enableAppBacktraceLogger and not engine::LogManager::isAppBacktraceLoggerEnabled()) {
        engine::LogManager::toggleAppBacktraceLogger(true);
      }
      if (not settings.enableAppBacktraceLogger and engine::LogManager::isAppBacktraceLoggerEnabled()) {
        engine::LogManager::toggleAppBacktraceLogger(false);
      }
      if (ImGui::Button("Clear all backtrace logger")) {
        engine::LogManager::clearAllBacktraceLogger();
      }
      if (ImGui::Button("Clear engine backtrace logger")) {
        engine::LogManager::clearEngineBacktraceLogger();
      }
      if (ImGui::Button("Clear app backtrace logger")) {
        engine::LogManager::clearAppBacktraceLogger();
      }
      if (ImGui::Button("Dump backtrace")) {
        engine::LogManager::DumpBacktrace();
      }
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
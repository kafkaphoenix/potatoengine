#pragma once

#include <imgui.h>
#include <imgui_stdlib.h>

#include "core/application.h"
#include "core/logManager.h"
#include "core/settingsManager.h"
#include "pch.h"
#include "render/renderManager.h"
#include "ui/imutils.h"

namespace potatoengine::ui {

std::string selectedSettingsManagerTabKey;

inline void
drawSettingsManager(const std::unique_ptr<SettingsManager>& settings_manager,
                    const std::unique_ptr<RenderManager>& render_manager) {
  ImGui::Columns(2);

  ImGui::SeparatorText("Engine");
  if (ImGui::Selectable("Window")) {
    selectedSettingsManagerTabKey = "Window";
  }
  if (ImGui::Selectable("Debug")) {
    selectedSettingsManagerTabKey = "Debug";
  }
  if (ImGui::Selectable("Logger")) {
    selectedSettingsManagerTabKey = "Logger";
  }
  if (ImGui::Selectable("Render")) {
    selectedSettingsManagerTabKey = "Render";
  }
  if (ImGui::Selectable("Scene")) {
    selectedSettingsManagerTabKey = "Scene";
  }

  auto& app = Application::Get();
  bool isGamePaused = app.isGamePaused();

  ImGui::NextColumn();
  if (not selectedSettingsManagerTabKey.empty()) {
    ImGui::SeparatorText("Edit");
    if (selectedSettingsManagerTabKey == "Window") {
      const auto& windows_manager = Application::Get().getWindowsManager();
      ImGui::Checkbox("Fullscreen", &settings_manager->fullscreen);
      windows_manager->toggleFullscreen(settings_manager->fullscreen);

      if (render_manager->getFramebuffersCount() == 0) {
        ImGui::BeginDisabled();
      }
      ImGui::Checkbox("Window inside Imgui",
                      &settings_manager->windowInsideImgui);
      if (render_manager->getFramebuffersCount() == 0) {
        ImGui::EndDisabled();
      }
      ImGui::SameLine();
      helpMark("Only works with a framebuffer");
      windows_manager->toggleWindowInsideImgui(
        settings_manager->windowInsideImgui);
      if (render_manager->getFramebuffersCount() == 0) {
        ImGui::BeginDisabled();
      }
      ImGui::Checkbox("Keep ratio", &settings_manager->fitToWindow);
      if (render_manager->getFramebuffersCount() == 0) {
        ImGui::EndDisabled();
      }
      ImGui::SameLine();
      helpMark("Only works with a framebuffer");
      windows_manager->toggleFitToWindow(settings_manager->fitToWindow);

      bool isFullscreen = settings_manager->fullscreen;
      if (isFullscreen) {
        ImGui::BeginDisabled();
      }
      ImGui::InputInt("Window width", &settings_manager->windowWidth);
      if (isFullscreen) {
        ImGui::EndDisabled();
      }
      ImGui::SameLine();
      helpMark("Only works in windowed mode");
      if (isFullscreen) {
        ImGui::BeginDisabled();
      }
      ImGui::InputInt("Window height", &settings_manager->windowHeight);
      if (isFullscreen) {
        ImGui::EndDisabled();
      }
      ImGui::SameLine();
      helpMark("Only works in windowed mode");
      windows_manager->resize(settings_manager->windowWidth,
                              settings_manager->windowHeight);
      if (isFullscreen) {
        ImGui::BeginDisabled();
      }
      ImGui::Checkbox("Resizable", &settings_manager->resizable);
      if (isFullscreen) {
        ImGui::EndDisabled();
      }
      ImGui::SameLine();
      helpMark("Only works in windowed mode");
      windows_manager->toggleResizable(settings_manager->resizable);

      if (not isFullscreen) {
        ImGui::BeginDisabled();
      }
      ImGui::InputInt("Refresh rate", &settings_manager->refreshRate);
      if (not isFullscreen) {
        ImGui::EndDisabled();
      }
      ImGui::SameLine();
      helpMark("Only works in fullscreen");
      windows_manager->setRefreshRate(settings_manager->refreshRate);

      ImGui::InputInt("Depth bits", &settings_manager->depthBits);
      ImGui::SameLine();
      helpMark("Requires restart");
      ImGui::Checkbox("VSync", &settings_manager->vSync);
      windows_manager->toggleVSync(settings_manager->vSync);
      ImGui::InputInt("Primary monitor", &settings_manager->primaryMonitor);
      windows_manager->setWindowMonitor(settings_manager->primaryMonitor);

      ImGui::SeparatorText("Info");
      ImGui::Text(std::format("OpenGL version {}.{}",
                              settings_manager->openglMajorVersion,
                              settings_manager->openglMinorVersion)
                    .c_str());
      const auto& windowData = windows_manager->getWindowData();
      ImGui::Text(std::format("Window position x:{}, y:{}",
                              windowData.positionX, windowData.positionY)
                    .c_str());
      ImGui::Text(std::format("Mouse position x:{}, y:{}",
                              windowData.debugMouseX, windowData.debugMouseY)
                    .c_str());

    } else if (selectedSettingsManagerTabKey ==
               "Debug") {
      ImGui::Checkbox("Game paused", &isGamePaused);
      app.togglePauseGame(isGamePaused);
      ImGui::Checkbox("Debug enabled", &settings_manager->debugEnabled); // TODO use for something
      ImGui::Checkbox("Display FPS", &settings_manager->displayFPS); // TODO use for something
      ImGui::Checkbox("Display collision boxes",
                      &settings_manager->displayCollisionBoxes);
    } else if (selectedSettingsManagerTabKey == "Logger") {
      ImGui::Checkbox("Enable engine logger",
                      &settings_manager->enableEngineLogger);
      LogManager::ToggleEngineLogger(settings_manager->enableEngineLogger);
      if (ImGui::BeginCombo("Engine log level",
                            settings_manager->engineLogLevel == 0   ? "Trace"
                            : settings_manager->engineLogLevel == 1 ? "Debug"
                            : settings_manager->engineLogLevel == 2 ? "Info"
                            : settings_manager->engineLogLevel == 3 ? "Warning"
                            : settings_manager->engineLogLevel == 4 ? "Error"
                            : settings_manager->engineLogLevel == 5
                              ? "Critical"
                              : "Unknown")) {
        for (uint32_t n = 0; n < settings_manager->logLevels.size(); n++) {
          bool is_selected = (settings_manager->engineLogLevel == n);
          if (ImGui::Selectable(settings_manager->logLevels[n], is_selected)) {
            if (settings_manager->engineLogLevel not_eq n) {
              settings_manager->engineLogLevel = n;
              LogManager::SetEngineLoggerLevel(
                static_cast<spdlog::level::level_enum>(n));
            }
          }
          if (is_selected) {
            ImGui::SetItemDefaultFocus();
          }
        }
        ImGui::EndCombo();
      }
      if (ImGui::BeginCombo(
            "Engine log flush level",
            settings_manager->engineFlushLevel == 0   ? "Trace"
            : settings_manager->engineFlushLevel == 1 ? "Debug"
            : settings_manager->engineFlushLevel == 2 ? "Info"
            : settings_manager->engineFlushLevel == 3 ? "Warning"
            : settings_manager->engineFlushLevel == 4 ? "Error"
            : settings_manager->engineFlushLevel == 5 ? "Critical"
                                                      : "Unknown")) {
        for (uint32_t n = 0; n < settings_manager->logLevels.size(); n++) {
          bool is_selected = (settings_manager->engineFlushLevel == n);
          if (ImGui::Selectable(settings_manager->logLevels[n], is_selected)) {
            if (settings_manager->engineFlushLevel not_eq n) {
              settings_manager->engineFlushLevel = n;
              LogManager::SetEngineLoggerFlushLevel(
                static_cast<spdlog::level::level_enum>(n));
            }
          }
          if (is_selected) {
            ImGui::SetItemDefaultFocus();
          }
        }
        ImGui::EndCombo();
      }
      ImGui::Checkbox("Enable app logger", &settings_manager->enableAppLogger);
      LogManager::ToggleAppLogger(settings_manager->enableAppLogger);
      if (ImGui::BeginCombo("App log level",
                            settings_manager->appLogLevel == 0   ? "Trace"
                            : settings_manager->appLogLevel == 1 ? "Debug"
                            : settings_manager->appLogLevel == 2 ? "Info"
                            : settings_manager->appLogLevel == 3 ? "Warning"
                            : settings_manager->appLogLevel == 4 ? "Error"
                            : settings_manager->appLogLevel == 5 ? "Critical"
                                                                 : "Unknown")) {
        for (uint32_t n = 0; n < settings_manager->logLevels.size(); n++) {
          bool is_selected = (settings_manager->appLogLevel == n);
          if (ImGui::Selectable(settings_manager->logLevels[n], is_selected)) {
            if (settings_manager->appLogLevel not_eq n) {
              settings_manager->appLogLevel = n;
              LogManager::SetAppLoggerLevel(
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
                            settings_manager->appFlushLevel == 0   ? "Trace"
                            : settings_manager->appFlushLevel == 1 ? "Debug"
                            : settings_manager->appFlushLevel == 2 ? "Info"
                            : settings_manager->appFlushLevel == 3 ? "Warning"
                            : settings_manager->appFlushLevel == 4 ? "Error"
                            : settings_manager->appFlushLevel == 5
                              ? "Critical"
                              : "Unknown")) {
        for (uint32_t n = 0; n < settings_manager->logLevels.size(); n++) {
          bool is_selected = (settings_manager->appFlushLevel == n);
          if (ImGui::Selectable(settings_manager->logLevels[n], is_selected)) {
            if (settings_manager->appFlushLevel not_eq n) {
              settings_manager->appFlushLevel = n;
              LogManager::SetAppLoggerFlushLevel(
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
                      &settings_manager->enableEngineBacktraceLogger);
      LogManager::ToggleEngineBacktraceLogger(
        settings_manager->enableEngineBacktraceLogger);
      ImGui::Checkbox("Enable app backtrace logger",
                      &settings_manager->enableAppBacktraceLogger);
      LogManager::ToggleAppBacktraceLogger(
        settings_manager->enableAppBacktraceLogger);
      if (ImGui::Button("Clear all backtrace logger")) {
        LogManager::ClearAllBacktraceLogger();
      }
      if (ImGui::Button("Clear engine backtrace logger")) {
        LogManager::ClearEngineBacktraceLogger();
      }
      if (ImGui::Button("Clear app backtrace logger")) {
        LogManager::ClearAppBacktraceLogger();
      }
      if (ImGui::Button("Dump backtrace")) {
        LogManager::DumpBacktrace();
      }
    } else if (selectedSettingsManagerTabKey == "Render") {
      ImGui::ColorEdit4("Clear color", settings_manager->clearColor.data());
      ImGui::SliderFloat("Clear depth", &settings_manager->clearDepth, 0.f,
                         1.f);
    } else if (selectedSettingsManagerTabKey == "Scene") {
      ImGui::Text("Name: %s", settings_manager->activeScene.c_str());
      ImGui::Text("Filepath: %s ", settings_manager->activeScenePath.c_str());
      if (ImGui::BeginCombo("Active scene",
                            settings_manager->activeScene.c_str())) {
        for (uint32_t n = 0; n < settings_manager->scenes.size(); n++) {
          bool is_selected =
            (settings_manager->activeScene == settings_manager->scenes[n]);
          if (ImGui::Selectable(settings_manager->scenes[n], is_selected)) {
            if (settings_manager->activeScene not_eq
                settings_manager->scenes[n]) {
              settings_manager->reloadScene = true;
              settings_manager->activeScene = settings_manager->scenes[n];
              settings_manager->activeScenePath =
                settings_manager->scenesPaths[n];
            }
          }
          if (is_selected) {
            ImGui::SetItemDefaultFocus();
          }
        }
        ImGui::EndCombo();
      }
      ImGui::Checkbox("Reload scene", &settings_manager->reloadScene);
    }
  }

  ImGui::Columns(1);
}
}
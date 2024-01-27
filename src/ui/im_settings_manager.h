#pragma once

#include <imgui.h>
#include <imgui_stdlib.h>

#include "core/application.h"
#include "core/logManager.h"
#include "pch.h"
#include "settings.h"
#include "ui/imutils.h"

namespace potatoengine::ui {

std::string selectedSettingsManagerTabKey;

inline void drawSettingsManager(const std::unique_ptr<Settings>& settings,
                                const std::unique_ptr<Renderer>& renderer) {
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
  ImGui::SeparatorText("App");
  if (ImGui::Selectable("Renderer")) {
    selectedSettingsManagerTabKey = "Renderer";
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
      const auto& window = Application::Get().getWindow();
      ImGui::Checkbox("Fullscreen", &settings->fullscreen);
      window->toggleFullscreen(settings->fullscreen);

      if (renderer->getFramebuffersCount() == 0) {
        ImGui::BeginDisabled();
      }
      ImGui::Checkbox("Window inside Imgui", &settings->windowInsideImgui);
      if (renderer->getFramebuffersCount() == 0) {
        ImGui::EndDisabled();
      }
      ImGui::SameLine();
      helpMark("Only works with a framebuffer");
      window->toggleWindowInsideImgui(settings->windowInsideImgui);
      if (renderer->getFramebuffersCount() == 0) {
        ImGui::BeginDisabled();
      }
      ImGui::Checkbox("Keep ratio", &settings->fitToWindow);
      if (renderer->getFramebuffersCount() == 0) {
        ImGui::EndDisabled();
      }
      ImGui::SameLine();
      helpMark("Only works with a framebuffer");
      window->toggleFitToWindow(settings->fitToWindow);

      bool isFullscreen = settings->fullscreen;
      if (isFullscreen) {
        ImGui::BeginDisabled();
      }
      ImGui::InputInt("Window width", &settings->windowWidth);
      if (isFullscreen) {
        ImGui::EndDisabled();
      }
      ImGui::SameLine();
      helpMark("Only works in windowed mode");
      if (isFullscreen) {
        ImGui::BeginDisabled();
      }
      ImGui::InputInt("Window height", &settings->windowHeight);
      if (isFullscreen) {
        ImGui::EndDisabled();
      }
      ImGui::SameLine();
      helpMark("Only works in windowed mode");
      window->resize(settings->windowWidth, settings->windowHeight);
      if (isFullscreen) {
        ImGui::BeginDisabled();
      }
      ImGui::Checkbox("Resizable", &settings->resizable);
      if (isFullscreen) {
        ImGui::EndDisabled();
      }
      ImGui::SameLine();
      helpMark("Only works in windowed mode");
      window->toggleResizable(settings->resizable);

      if (not isFullscreen) {
        ImGui::BeginDisabled();
      }
      ImGui::InputInt("Refresh rate", &settings->refreshRate);
      if (not isFullscreen) {
        ImGui::EndDisabled();
      }
      ImGui::SameLine();
      helpMark("Only works in fullscreen");
      window->setRefreshRate(settings->refreshRate);

      ImGui::InputInt("Depth bits", &settings->depthBits);
      ImGui::SameLine();
      helpMark("Requires restart");
      ImGui::Checkbox("VSync", &settings->vSync);
      window->toggleVSync(settings->vSync);
      ImGui::InputInt("Primary monitor", &settings->primaryMonitor);
      window->setWindowMonitor(settings->primaryMonitor);

      ImGui::SeparatorText("Info");
      ImGui::Text(std::format("OpenGL version {}.{}",
                              settings->openglMajorVersion,
                              settings->openglMinorVersion)
                    .c_str());
      const auto& windowData = window->getWindowData();
      ImGui::Text(std::format("Window position x:{}, y:{}",
                              windowData.positionX, windowData.positionY)
                    .c_str());
      ImGui::Text(std::format("Mouse position x:{}, y:{}",
                              windowData.debugMouseX, windowData.debugMouseY)
                    .c_str());

    } else if (selectedSettingsManagerTabKey ==
               "Debug") { // TODO use for something
      ImGui::Checkbox("Game paused", &isGamePaused);
      app.togglePauseGame(isGamePaused);
      ImGui::Checkbox("Debug enabled", &settings->debugEnabled);
      ImGui::Checkbox("Display FPS", &settings->displayFPS);
    } else if (selectedSettingsManagerTabKey == "Logger") {
      ImGui::Checkbox("Enable engine logger", &settings->enableEngineLogger);
      LogManager::ToggleEngineLogger(settings->enableEngineLogger);
      if (ImGui::BeginCombo("Engine log level",
                            settings->engineLogLevel == 0   ? "Trace"
                            : settings->engineLogLevel == 1 ? "Debug"
                            : settings->engineLogLevel == 2 ? "Info"
                            : settings->engineLogLevel == 3 ? "Warning"
                            : settings->engineLogLevel == 4 ? "Error"
                            : settings->engineLogLevel == 5 ? "Critical"
                                                            : "Unknown")) {
        for (int n = 0; n < settings->logLevels.size(); n++) {
          bool is_selected = (settings->engineLogLevel == n);
          if (ImGui::Selectable(settings->logLevels[n], is_selected)) {
            if (settings->engineLogLevel not_eq n) {
              settings->engineLogLevel = n;
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
      if (ImGui::BeginCombo("Engine log flush level",
                            settings->engineFlushLevel == 0   ? "Trace"
                            : settings->engineFlushLevel == 1 ? "Debug"
                            : settings->engineFlushLevel == 2 ? "Info"
                            : settings->engineFlushLevel == 3 ? "Warning"
                            : settings->engineFlushLevel == 4 ? "Error"
                            : settings->engineFlushLevel == 5 ? "Critical"
                                                              : "Unknown")) {
        for (int n = 0; n < settings->logLevels.size(); n++) {
          bool is_selected = (settings->engineFlushLevel == n);
          if (ImGui::Selectable(settings->logLevels[n], is_selected)) {
            if (settings->engineFlushLevel not_eq n) {
              settings->engineFlushLevel = n;
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
      ImGui::Checkbox("Enable app logger", &settings->enableAppLogger);
      LogManager::ToggleAppLogger(settings->enableAppLogger);
      if (ImGui::BeginCombo("App log level",
                            settings->appLogLevel == 0   ? "Trace"
                            : settings->appLogLevel == 1 ? "Debug"
                            : settings->appLogLevel == 2 ? "Info"
                            : settings->appLogLevel == 3 ? "Warning"
                            : settings->appLogLevel == 4 ? "Error"
                            : settings->appLogLevel == 5 ? "Critical"
                                                         : "Unknown")) {
        for (int n = 0; n < settings->logLevels.size(); n++) {
          bool is_selected = (settings->appLogLevel == n);
          if (ImGui::Selectable(settings->logLevels[n], is_selected)) {
            if (settings->appLogLevel not_eq n) {
              settings->appLogLevel = n;
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
                            settings->appFlushLevel == 0   ? "Trace"
                            : settings->appFlushLevel == 1 ? "Debug"
                            : settings->appFlushLevel == 2 ? "Info"
                            : settings->appFlushLevel == 3 ? "Warning"
                            : settings->appFlushLevel == 4 ? "Error"
                            : settings->appFlushLevel == 5 ? "Critical"
                                                           : "Unknown")) {
        for (int n = 0; n < settings->logLevels.size(); n++) {
          bool is_selected = (settings->appFlushLevel == n);
          if (ImGui::Selectable(settings->logLevels[n], is_selected)) {
            if (settings->appFlushLevel not_eq n) {
              settings->appFlushLevel = n;
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
                      &settings->enableEngineBacktraceLogger);
      LogManager::ToggleEngineBacktraceLogger(
        settings->enableEngineBacktraceLogger);
      ImGui::Checkbox("Enable app backtrace logger",
                      &settings->enableAppBacktraceLogger);
      LogManager::ToggleAppBacktraceLogger(settings->enableAppBacktraceLogger);
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
    } else if (selectedSettingsManagerTabKey == "Renderer") {
      ImGui::ColorEdit4("Clear color", settings->clearColor.data());
      ImGui::SliderFloat("Clear depth", &settings->clearDepth, 0.f, 1.f);
    } else if (selectedSettingsManagerTabKey == "Scene") {
      ImGui::Text("Name: %s", settings->activeScene.c_str());
      ImGui::Text("Filepath: %s ", settings->activeScenePath.c_str());
      if (ImGui::BeginCombo("Active scene", settings->activeScene.c_str())) {
        for (int n = 0; n < settings->scenes.size(); n++) {
          bool is_selected = (settings->activeScene == settings->scenes[n]);
          if (ImGui::Selectable(settings->scenes[n], is_selected)) {
            if (settings->activeScene not_eq settings->scenes[n]) {
              settings->reloadScene = true;
              settings->activeScene = settings->scenes[n];
              settings->activeScenePath = settings->scenesPaths[n];
            }
          }
          if (is_selected) {
            ImGui::SetItemDefaultFocus();
          }
        }
        ImGui::EndCombo();
      }
      ImGui::Checkbox("Reload scene", &settings->reloadScene);
    }
  }

  ImGui::Columns(1);
}
}
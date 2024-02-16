#include "imgui/imguiLayer.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_stdlib.h>

#include "core/application.h"
#include "imgui/imdebugger.h"
#include "imgui/imutils.h"
#include "pch.h"

namespace potatoengine {

ImGuiLayer::ImGuiLayer() : Layer("ImGuiLayer") {}

void ImGuiLayer::onAttach() {
  ENGINE_TRACE("Initializing ImGuiLayer");
  auto& app = Application::Get();
  const auto& settings_manager = app.getSettingsManager();
  std::string glsl_version =
    std::format("#version {}{}0", settings_manager->openglMajorVersion,
                settings_manager->openglMinorVersion);
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
  io.ConfigWindowsMoveFromTitleBarOnly = true;

  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(app.getWindowsManager()->getNativeWindow(),
                               true);
  ImGui_ImplOpenGL3_Init(glsl_version.data());

  ImGuiStyle& style = ImGui::GetStyle();
  style.WindowRounding = 5.f;
  style.FrameRounding = 5.f;
  style.PopupRounding = 5.f;
}

void ImGuiLayer::begin() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}

void ImGuiLayer::end() {
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiLayer::onImguiUpdate() {
  auto& app = Application::Get();

  if (app.isDebugging()) {
    const auto& settings_manager = app.getSettingsManager();
    const auto& assets_manager = app.getAssetsManager();
    const auto& render_manager = app.getRenderManager();
    const auto& scene_manager = app.getSceneManager();
    const auto& states_manager = app.getStatesManager();
    drawDebugger(settings_manager, assets_manager, render_manager,
                 scene_manager, states_manager);
  }
}

void ImGuiLayer::onDetach() {
  ENGINE_WARN("Shutting down imgui api");
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}
}
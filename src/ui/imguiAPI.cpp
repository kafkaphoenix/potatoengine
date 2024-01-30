#include "ui/imguiAPI.h"

namespace potatoengine::ui {
void ImGuiAPI::Init(GLFWwindow* window, uint32_t openglMajorVersion,
                    uint32_t openglMinorVersion) {
  ENGINE_TRACE("Initializing ImGuiAPI");
  std::string glsl_version =
    std::format("#version {}{}0", openglMajorVersion, openglMinorVersion);
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
  io.ConfigWindowsMoveFromTitleBarOnly = true;

  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(glsl_version.data());

  ImGuiStyle& style = ImGui::GetStyle();
  style.WindowRounding = 5.f;
  style.FrameRounding = 5.f;
  style.PopupRounding = 5.f;
}

void ImGuiAPI::NewFrame() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}

void ImGuiAPI::Render() {
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiAPI::Shutdown() {
  ENGINE_WARN("Shutting down imgui api");
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}
}
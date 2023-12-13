#include "ui/imguiAPI.h"

namespace potatoengine {
void ImGuiAPI::Init(GLFWwindow* window, int openglMajorVersion, int openglMinorVersion) {
  ENGINE_INFO("Initializing ImGuiAPI");
  std::string glsl_version = std::format("#version {}{}0", openglMajorVersion, openglMinorVersion);
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(glsl_version.data());
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
  ENGINE_WARN("Shutting down ImGuiAPI");
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}
}
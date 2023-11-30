#pragma once

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_stdlib.h>

#include "potatoengine/ui/imutils.h"

namespace potatoengine {
class ImGuiAPI {
   public:
    static void Init(GLFWwindow* window, int openglMajorVersion, int openglMinorVersion);
    static void NewFrame();
    static void Render();
    static void Shutdown();
};
}
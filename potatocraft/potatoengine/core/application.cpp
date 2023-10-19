#include "potatoengine/core/application.h"

#include "potatoengine/assets/assetsManager.h"
#include "potatoengine/core/time.h"
#include "potatoengine/renderer/renderer.h"
// #include <imgui.h>
// #include <imgui_impl_glfw.h>
// #include <imgui_impl_opengl3.h>

namespace potatoengine {

// struct IMGUI_STATES
// {
//     bool show_demo_window = true;
//     bool show_another_window{};
// } imgui_debugger;

// Function prototypes
// void teardown(GLFWwindow *window);
// void init_imgui_context(GLFWwindow *window, const char *glsl_version);
// void debugger(IMGUI_STATES states, GLFWwindow *window, ImVec4 clear_color);

Application::Application(Config&& c, CLArgs&& args)
    : m_name(std::move(c.name)), m_clargs(std::move(args)) {
    s_instance = this;

    std::filesystem::current_path(c.root);
    m_states = StateStack::Create();
    m_assetsManager = AssetsManager::Create();
    m_window = Window::Create(WindowProperties{.title = c.name, .width = c.width, .height = c.height});
    m_window->setEventCallback(BIND_EVENT(Application::onEvent));

    m_renderer = Renderer::Create(m_assetsManager);
    m_renderer->init();
    // TODO class imgui layer with init and push overlay
}

Application::~Application() {
#ifdef DEBUG
    CORE_INFO("Deleting Application");
#endif
    m_renderer->shutdown();
    // TODO class imgui layer with shutdown and pop overlay
}

bool Application::onWindowClose(WindowCloseEvent&) {
    terminate();

    return true;
}

bool Application::onWindowResize(WindowResizeEvent& e) {
    if (e.getWidth() == 0 or e.getHeight() == 0) {
        m_minimized = true;
    } else {
        m_minimized = false;
        m_renderer->onWindowResize(e.getWidth(), e.getHeight());
    }

    return true;
}

void Application::onEvent(Event& e) {
    EventDispatcher dispatcher(e);
    dispatcher.dispatch<WindowCloseEvent>(BIND_EVENT(Application::onWindowClose));
    dispatcher.dispatch<WindowResizeEvent>(BIND_EVENT(Application::onWindowResize));

    for (auto it = m_states->rbegin(); it not_eq m_states->rend() and not e.m_handled; ++it) {
        (*it)->onEvent(e);
    }
}

void Application::pushState(std::unique_ptr<State>&& s) {
    s->onAttach();
    m_states->pushState(std::move(s));
}

void Application::pushOverlay(std::unique_ptr<State>&& s) {
    s->onAttach();
    m_states->pushOverlay(std::move(s));
}

void Application::run() {
    // const char *glsl_version = "#version 460";
    // GLFWwindow* window = static_cast<GLFWwindow*>(getWindow().getNativeWindow());

    // init_imgui_context(window, glsl_version);

    // ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.6f, 1.f);

    while (m_running) {
        float currentFrame = (float)glfwGetTime();
        Time ts = currentFrame - m_lastFrame;
        m_lastFrame = currentFrame;
        m_accumulator += ts;

        if (not m_minimized) [[likely]] {
            while (m_accumulator > ts) {
                for (auto& state : *m_states) {
                    state->onUpdate(ts);
                }
                m_accumulator -= ts;
                if (m_accumulator < 0) {
                    m_accumulator = 0;
                }
            }

            /*if (m_debugging) {
                debugger(imgui_debugger, window, clear_color);
                ImGui::Render();
                ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            }*/
        }
        // TODO: open menu pause
#ifdef DEBUG
        // CORE_INFO("FPS: {}", 1.f / ts);
        //  TODO move to imgui debug panel
#endif
        m_window->onUpdate();
    }
    // espera teardown(window);
}

/*void teardown(GLFWwindow *window)
{
    // espera esto se va al renderer para imgui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void init_imgui_context(GLFWwindow *window, const char *glsl_version)
{
    std::print("Starting ImGui context\n");
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    // ImFont* font =
io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\UDDigiKyokashoN-B.ttc", 18.f,
nullptr, io.Fonts->GetGlyphRangesJapanese());
    // IM_ASSERT(font);
}

void debugger(IMGUI_STATES states, GLFWwindow *window, ImVec4 clear_color)
{
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // 1. Show the big demo window (Most of the sample code is in
ImGui::ShowDemoWindow()! You can browe its code to learn more about Dear
ImGui!). if (states.show_demo_window)
        ImGui::ShowDemoWindow(&states.show_demo_window);

    // 2. Show a simple window that we create ourselves. We use a Begin/End pair
to created a named window.
    {
        static float f = 0.f;
        static int counter = 0;

        ImGui::Begin("Hello, world!"); // Create a window called "Hello, world!"
and append into it.

        ImGui::Text("This is some useful text.");                 // Display
some text (you can use a format strings too) ImGui::Checkbox("Demo Window",
&states.show_demo_window); // Edit bools storing our window open/close state
        ImGui::Checkbox("Another Window", &states.show_another_window);

        ImGui::SliderFloat("float", &f, 0.f, 1.f);             // Edit 1 float
using a slider from 0.f to 1.f ImGui::ColorEdit3("clear color", (float
*)&clear_color); // Edit 3 floats representing a color

        if (ImGui::Button("Button")) // Buttons return true when clicked (most
widgets return true when edited/activated)
            ++counter;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.f /
ImGui::GetIO().Framerate, ImGui::GetIO().Framerate); ImGui::End();
    }

    // 3. Show another simple window.
    if (states.show_another_window)
    {
        ImGui::Begin("Another Window", &states.show_another_window); // Pass a
pointer to our bool variable (the window will have a closing button that will
clear the bool when clicked) ImGui::Text("Hello from another window!"); if
(ImGui::Button("Close Me")) states.show_another_window = false; ImGui::End();
    }
}*/

}
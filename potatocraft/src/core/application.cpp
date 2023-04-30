#include "src/pch.h"
#include "src/core/application.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "src/core/window.h"
#include "src/core/timestep.h"

#include "src/core/input.h"
#include "src/core/keyCodes.h"

#include "src/renderer/renderer.h"

namespace potatoengine
{

    struct IMGUI_STATES
    {
        bool show_demo_window = true;
        bool show_another_window = false;
    } imgui_debugger;

    // Function prototypes
    void teardown(GLFWwindow *window);
    void init_imgui_context(GLFWwindow *window, const char *glsl_version);
    void debugger(IMGUI_STATES states, GLFWwindow *window, ImVec4 clear_color);
    //void movement(GLFWwindow *window);

    Application::Application(const std::string &name, CommandLineArgs args)
        : m_name(name), m_commandLineArgs(args)
    {
        s_instance = this;
    
        m_window = Window::Create(WindowProperties(m_name));
		m_window->setEventCallback(BIND_EVENT_FN(Application::onEvent));

		Renderer::Init();
        // TODO class imgui init
    }

    Application::~Application()
    {
        Renderer::Shutdown();
    }

	void Application::close()
	{
		m_running = false;
	}

    bool Application::onKeyPressed(KeyPressedEvent& e) {
        switch (e.getKeyCode())
		{
        case Key::Escape:
		    m_running = false;
            break;
        case Key::F3:
		    m_debugging = not m_debugging;
            break;
        case Key::F4:
		    m_wireframe = not m_wireframe;
            break;
        }
        return true;
    }

    bool Application::onWindowClose(WindowCloseEvent& e)
	{
		m_running = false;
		return true;
	}

	bool Application::onWindowResize(WindowResizeEvent& e)
	{
		if (e.getWidth() == 0 or e.getHeight() == 0)
		{
			m_minimized = true;
		} else {
            m_minimized = false;
		    Renderer::OnWindowResize(e.getWidth(), e.getHeight());
        }

		return false;
	}

    void Application::onEvent(Event& e) {
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::onWindowClose));
		dispatcher.dispatch<WindowResizeEvent>(BIND_EVENT_FN(Application::onWindowResize));
		dispatcher.dispatch<KeyPressedEvent>(BIND_EVENT_FN(Application::onKeyPressed));

		for (auto it = m_stateStack.rbegin(); it not_eq m_stateStack.rend(); ++it)
		{
			if (e.m_handled) 
				break;
			(*it)->onEvent(e);
		}
    }

    void Application::pushState(State* state) {
        m_stateStack.pushState(state);
		state->onAttach();
    }

    void Application::pushOverlay(State* state) {
        m_stateStack.pushOverlay(state);
		state->onAttach();
    }

    void Application::run()
    {
        const char *glsl_version = "#version 460";
        auto* window = static_cast<GLFWwindow*>(getWindow().getNativeWindow());

        init_imgui_context(window, glsl_version);

        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

        while (m_running)
        {
			Timestep dt = glfwGetTime() - m_lastFrame;
			m_lastFrame = glfwGetTime();
            m_accumulator += dt;

            while (m_accumulator > 1.0/61.0) {
                for (auto state : m_stateStack)
					state->onUpdate(dt);
                m_accumulator -= 1.0/59.0;
                if (m_accumulator < 0) m_accumulator = 0;
            }

            if (m_debugging)
                debugger(imgui_debugger, window, clear_color);

            // Rendering ImGui Layer
            if (m_debugging)
                ImGui::Render();

            RendererAPI::SetWireframe(m_wireframe);

            // espera - process input - movement((GLFWwindow*)GetWindow().getNativeWindow());

            if (m_debugging)
                ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            m_window->onUpdate();
        }
        // espera teardown(window);
    }

    /* espera void movement(GLFWwindow *window)
    {
        if (glfwGetKey(window, GLFW_KEY_W))
            cam.processKeyboard(Camera::CameraMovement::FORWARD, dt);
        if (glfwGetKey(window, GLFW_KEY_S))
            cam.processKeyboard(Camera::CameraMovement::BACKWARD, dt);
        if (glfwGetKey(window, GLFW_KEY_A))
            cam.processKeyboard(Camera::CameraMovement::LEFT, dt);
        if (glfwGetKey(window, GLFW_KEY_D))
            cam.processKeyboard(Camera::CameraMovement::RIGHT, dt);
        if (glfwGetKey(window, GLFW_KEY_SPACE))
            cam.processKeyboard(Camera::CameraMovement::JUMP, dt);
        if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL))
            cam.processKeyboard(Camera::CameraMovement::CROUCH, dt);
    }*/

    void teardown(GLFWwindow *window)
    {
        // espera esto se va al renderer para imgui
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void init_imgui_context(GLFWwindow *window, const char *glsl_version)
    {
        fprintf(stdout, "Starting ImGui context\n");
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init(glsl_version);
        // ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\UDDigiKyokashoN-B.ttc", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
        // IM_ASSERT(font not_eq nullptr);
    }

    void debugger(IMGUI_STATES states, GLFWwindow *window, ImVec4 clear_color)
    {
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (states.show_demo_window)
            ImGui::ShowDemoWindow(&states.show_demo_window);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!"); // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");                 // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window", &states.show_demo_window); // Edit bools storing our window open/close state
            ImGui::Checkbox("Another Window", &states.show_another_window);

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);             // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float *)&clear_color); // Edit 3 floats representing a color

            if (ImGui::Button("Button")) // Buttons return true when clicked (most widgets return true when edited/activated)
                ++counter;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        // 3. Show another simple window.
        if (states.show_another_window)
        {
            ImGui::Begin("Another Window", &states.show_another_window); // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                states.show_another_window = false;
            ImGui::End();
        }
    }

}
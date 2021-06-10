#include <iostream>
#include <string>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <glm/vec3.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <entt/entt.hpp>

#include <nlohmann/json.hpp>

#define STB_IMAGE_IMPLEMENTATION    
#define STBI_FAILURE_USERMSG
#include <stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include <filesystem>
namespace fs = std::filesystem;

#include <src/shader/program.hpp>

#include "GLDebugMessageCallback.h"

using json = nlohmann::json;

struct IMGUI_STATES 
{
    bool show_demo_window = true;
    bool show_another_window = false;
} imgui_debugger;

const bool DEBUGGER_ENABLED = false;
bool debugger_enabled = DEBUGGER_ENABLED;
const bool WIREFRAME_ENABLED = false;
bool wireframe_enabled = WIREFRAME_ENABLED;

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
static void glfw_error_callback(int error, const char *description);
void teardown(GLFWwindow *window);
void debugger(IMGUI_STATES states, GLFWwindow *window, ImVec4 clear_color);
void load_shaders(Program& program);
void chunks(unsigned int& VBO, unsigned int& VAO, unsigned int& EBO);
void load_texture(unsigned int &texture);

// Window dimensions
const GLuint WIDTH = 1280, HEIGHT = 720;

int main() 
{
    fprintf(stdout, "Starting GLFW context, OpenGL 4.6\n");
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    const char* glsl_version = "#version 460";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    int monitorCount;
    GLFWmonitor** monitors = glfwGetMonitors(&monitorCount);

    int xpos = 50;
    int ypos = 50;
    if (monitorCount >= 2)
    {
        xpos = 500;
        ypos = 200;
    }

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "potatocraft", nullptr, nullptr);
    if (window == nullptr)
    {
        teardown(nullptr);
        return 1;
    }
    glfwSetWindowMonitor(window, nullptr, xpos, ypos, WIDTH, HEIGHT, 0);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        fprintf(stderr, "Failed to initialize OpenGL loader!\n");
        teardown(window);
        return 1;
    }

    // glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(GLDebugMessageCallback, nullptr);

    Program display;
    load_shaders(display);

    unsigned int VBO, VAO, EBO;
    chunks(VBO, VAO, EBO);

    unsigned int texture;
    load_texture(texture);

    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    display.use(); // don't forget to activate/use the shader before setting uniforms!
    display.setInt("texSampler", 0);

    fprintf(stdout, "Starting ImGui context\n");
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    // ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\UDDigiKyokashoN-B.ttc", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
    // IM_ASSERT(font != nullptr);

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Set the required callback functions
    glfwSetKeyCallback(window, key_callback);

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (debugger_enabled) debugger(imgui_debugger, window, clear_color);

        // Rendering
        if (debugger_enabled) ImGui::Render();
        if (wireframe_enabled) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        } else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        if (debugger_enabled) ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        display.use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    teardown(window);

    return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (key == GLFW_KEY_F3 && action == GLFW_PRESS)
        debugger_enabled = !(debugger_enabled);

    if (key == GLFW_KEY_F4 && action == GLFW_PRESS)
        wireframe_enabled = !(wireframe_enabled);
}

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

void teardown(GLFWwindow* window)
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    if (window != nullptr) 
        glfwDestroyWindow(window);
    glfwTerminate();
}

void debugger(IMGUI_STATES states, GLFWwindow* window, ImVec4 clear_color)
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

            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window", &states.show_demo_window);      // Edit bools storing our window open/close state
            ImGui::Checkbox("Another Window", &states.show_another_window);

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        // 3. Show another simple window.
        if (states.show_another_window)
        {
            ImGui::Begin("Another Window", &states.show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                states.show_another_window = false;
            ImGui::End();
        }
}

void load_shaders(Program& display)
{
    fprintf(stdout, "Creating display vertex shader.\n");
	Shader shader_vert;
    bool load_status_vert = false;
    shader_vert.load_file(GL_VERTEX_SHADER, "..\\assets\\shaders\\display.vert", load_status_vert);

    fprintf(stdout, "Creating display fragment shader.\n");
	Shader shader_frag;
    bool load_status_frag = false;
    shader_frag.load_file(GL_FRAGMENT_SHADER, "..\\assets\\shaders\\display.frag", load_status_frag);

    if (load_status_vert && load_status_frag) {
        fprintf(stdout, "Linking shader program.\n");
        display.attach(shader_vert);
        display.attach(shader_frag);
        bool link_status = false;
        display.link(link_status);
        if (link_status) {
            display.detach(shader_vert);
            display.detach(shader_frag);
        } else {
            shader_vert.~Shader();
            shader_frag.~Shader();
            display.~Program();
        }
    } else {
        fprintf(stdout, "Could not link shader program.\n");
        shader_vert.~Shader();
        shader_frag.~Shader();
        display.~Program();
    }
}

void chunks(unsigned int& VBO, unsigned int& VAO, unsigned int& EBO)
{
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        // positions          // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,   0.0f, 1.0f  // top left 
    };
    unsigned int indices[] = {  
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void load_texture(unsigned int& texture)
{
    // load and create a texture 
    // -------------------------
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    std::string filename_path = "..\\assets\\textures\\wall.jpg";
    unsigned char *data = stbi_load(filename_path.c_str(), &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        fprintf(stdout, "Failed to load texture: %s\n", stbi_failure_reason());
    }
    stbi_image_free(data);
}
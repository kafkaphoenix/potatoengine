#include "src/sandbox/gameState.h"

namespace potatocraft {

    // world space positions of our cubes
    glm::vec3 cubePositions[] = {
        {0.0f, 0.0f, 0.0f},
        {2.0f, 5.0f, -15.0f},
        {-1.5f, -2.2f, -2.5f},
        {-3.8f, -2.0f, -12.3f},
        {2.4f, -0.4f, -3.5f},
        {-1.7f, 3.0f, -7.5f},
        {1.3f, -2.0f, -2.5f},
        {1.5f, 2.0f, -2.5f},
        {1.5f, 0.2f, -1.5f},
        {-1.3f, 1.0f, -1.5f}
    };

    const GLfloat render_x = 3840.f, render_y = 2160.f; // TODO: REMOVE?

    void loadShader(potatoengine::Ref<potatoengine::Program> &m_shader) // todo move to class
    {
        fprintf(stdout, "Loading shader program\n");
        std::string s_name = m_shader->getName();
        fprintf(stdout, "Creating vertex shader\n");
        potatoengine::Shader shader_vert;
        bool load_status_vert = false;
        shader_vert.load_file(GL_VERTEX_SHADER, "..\\assets\\shaders\\" + s_name + ".vert", load_status_vert);

        fprintf(stdout, "Creating fragment shader\n");
        potatoengine::Shader shader_frag;
        bool load_status_frag = false;
        shader_frag.load_file(GL_FRAGMENT_SHADER, "..\\assets\\shaders\\" + s_name + ".frag", load_status_frag);

        if (load_status_vert and load_status_frag)
        {
            fprintf(stdout, "Attaching shaders\n");
            m_shader->attach(shader_vert);
            m_shader->attach(shader_frag);
            bool link_status = false;
            fprintf(stdout, "Linking shader program\n");
            m_shader->link(link_status);
            if (link_status)
            {
                m_shader->detach(shader_vert);
                m_shader->detach(shader_frag);
            }
            else
            {
                fprintf(stdout, "Could not link shader program\n");
                throw(std::runtime_error("Could not link shader program"));
            }
        }
        else
        {
            fprintf(stdout, "Could not load shader files\n");
            throw std::runtime_error("Failed to load file\n"); //todo move to assert
        }
    }

    void loadCubes(potatoengine::Ref<potatoengine::VAO> &m_vao) { // todo move to class
        fprintf(stdout, "Loading cubes\n");
        m_vao = potatoengine::VAO::Create();
        m_vao->bind();

        std::vector<potatoengine::Vertex> vertices = {
            {{-0.5f, -0.5f, -0.5f}, {0.3f, 0.15f, 0.06f, 1.0f}, {0.0f, 0.0f}},  // A 0
            {{0.5f, -0.5f, -0.5f}, {0.3f, 0.15f, 0.06f, 1.0f}, {1.0f, 0.0f}},   // B 1
            {{0.5f, 0.5f, -0.5f}, {0.3f, 0.15f, 0.06f, 1.0f}, {1.0f, 1.0f}},    // C 2
            {{-0.5f, 0.5f, -0.5f}, {0.3f, 0.15f, 0.06f, 1.0f}, {0.0f, 1.0f}},   // D 3
            {{-0.5f, -0.5f, 0.5f}, {0.3f, 0.15f, 0.06f, 1.0f}, {0.0f, 0.0f}},   // E 4
            {{0.5f, -0.5f, 0.5f}, {0.3f, 0.15f, 0.06f, 1.0f}, {1.0f, 0.0f}},    // F 5
            {{0.5f, 0.5f, 0.5f}, {0.3f, 0.15f, 0.06f, 1.0f}, {1.0f, 1.0f}},     // G 6
            {{-0.5f, 0.5f, 0.5f}, {0.3f, 0.15f, 0.06f, 1.0f}, {0.0f, 1.0f}},    // H 7

            {{-0.5f,  0.5f, -0.5f}, {0.3f, 0.15f, 0.06f, 1.0f}, {0.0f, 0.0f}},  // D 8
            {{-0.5f, -0.5f, -0.5f}, {0.3f, 0.15f, 0.06f, 1.0f}, {1.0f, 0.0f}},  // A 9
            {{-0.5f, -0.5f,  0.5f}, {0.3f, 0.15f, 0.06f, 1.0f}, {1.0f, 1.0f}},  // E 10
            {{-0.5f,  0.5f,  0.5f}, {0.3f, 0.15f, 0.06f, 1.0f}, {0.0f, 1.0f}},  // H 11
            {{ 0.5f, -0.5f, -0.5f}, {0.3f, 0.15f, 0.06f, 1.0f}, {0.0f, 0.0f}},  // B 12
            {{ 0.5f,  0.5f, -0.5f}, {0.3f, 0.15f, 0.06f, 1.0f}, {1.0f, 0.0f}},  // C 13
            {{ 0.5f,  0.5f,  0.5f}, {0.3f, 0.15f, 0.06f, 1.0f}, {1.0f, 1.0f}},  // G 14
            {{ 0.5f, -0.5f,  0.5f}, {0.3f, 0.15f, 0.06f, 1.0f}, {0.0f, 1.0f}},  // F 15

            {{-0.5f, -0.5f, -0.5f}, {0.3f, 0.15f, 0.06f, 1.0f}, {0.0f, 0.0f}}, // A 16
            {{ 0.5f, -0.5f, -0.5f}, {0.3f, 0.15f, 0.06f, 1.0f}, {1.0f, 0.0f}}, // B 17
            {{ 0.5f, -0.5f,  0.5f}, {0.3f, 0.15f, 0.06f, 1.0f}, {1.0f, 1.0f}}, // F 18
            {{-0.5f, -0.5f,  0.5f}, {0.3f, 0.15f, 0.06f, 1.0f}, {0.0f, 1.0f}}, // E 19
            {{ 0.5f,  0.5f, -0.5f}, {0.3f, 0.15f, 0.06f, 1.0f}, {0.0f, 0.0f}}, // C 20
            {{-0.5f,  0.5f, -0.5f}, {0.3f, 0.15f, 0.06f, 1.0f}, {1.0f, 0.0f}}, // D 21
            {{-0.5f,  0.5f,  0.5f}, {0.3f, 0.15f, 0.06f, 1.0f}, {1.0f, 1.0f}}, // H 22
            {{ 0.5f,  0.5f,  0.5f}, {0.3f, 0.15f, 0.06f, 1.0f}, {0.0f, 1.0f}}, // G 23
        };

        std::vector<uint32_t> indices = {
            // front and back
            0, 3, 2,
            2, 1, 0,
            4, 5, 6,
            6, 7 ,4,
            // left and right
            11, 8, 9,
            9, 10, 11,
            12, 13, 14,
            14, 15, 12,
            // bottom and top
            16, 17, 18,
            18, 19, 16,
            20, 21, 22,
            22, 23, 20
        };

        auto vbo = potatoengine::VBO::Create(vertices);
        auto ibo = potatoengine::IBO::Create(indices);

        m_vao->attachVertex(vbo);
        m_vao->setIndex(ibo);
    }


    GameState::GameState() : State("GameState"), m_camera({0.f, 0.f, -3.5f}, {0.f, 0.f, 0.f}) {
        m_shader = potatoengine::Program::Create("shader");
        loadShader(m_shader);

        m_shader->use();
        m_shader->setMat4("view", m_camera.getView());

        glm::mat4 projection = glm::perspective(glm::radians(m_camera.getFov()), render_x / render_y, 1.0f, 3000.0f);
        m_shader->setMat4("projection", projection);
        
        loadCubes(m_vao);

        m_texture = potatoengine::Texture::Create("..\\assets\\textures\\wall.jpg");

        // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
        m_shader->use(); // don't forget to activate/use the shader before setting uniforms!
        m_shader->setInt("texSampler", 0);
    }

    void GameState::onAttach() {

    }

    void GameState::onDetach() {

    }

    void GameState::onUpdate(potatoengine::Timestep ts) {
        potatoengine::RendererAPI::SetClearColor({0.45f, 0.55f, 0.60f, 1.00f});
        potatoengine::RendererAPI::Clear();

        potatoengine::Renderer::BeginScene(m_camera);

        m_texture->bind(0);

        m_shader->use();

        glm::mat4 projection = glm::perspective(
            glm::radians(m_camera.getFov()),
            render_x / render_y,
            1.0f,
            3000.0f
        );
        m_shader->setMat4("projection", projection);

        m_shader->setMat4("view", m_camera.getView());

        // render boxes
        for (uint32_t i = 0; i < 10; i++)
        {
            // calculate the model matrix for each object and pass it to shader before drawing
            glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
            model = glm::translate(model, cubePositions[i]);
            m_shader->setMat4("model", model);
            potatoengine::Renderer::Submit(m_shader, m_vao);
        }


        potatoengine::Renderer::EndScene();
    }

    void GameState::onImGuiRender() {

    }

    void GameState::onEvent(potatoengine::Event& event) {

    }

}
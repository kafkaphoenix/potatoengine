#include "sandbox/gameState.h"

namespace potatocraft {

// world space positions of our cubes // todo move to class
glm::vec3 cubePositions[] = {
    {0.f, 0.f, 0.f},
    {2.f, 5.f, -15.f},
    {-1.5f, -2.2f, -2.5f},
    {-3.8f, -2.f, -12.3f},
    {2.4f, -0.4f, -3.5f},
    {-1.7f, 3.f, -7.5f},
    {1.3f, -2.f, -2.5f},
    {1.5f, 2.f, -2.5f},
    {1.5f, 0.2f, -1.5f},
    {-1.3f, 1.f, -1.5f}};

GameState::GameState(const std::shared_ptr<potatoengine::AssetsManager> &assetsManager) : State("GameState"), m_cameraController(), m_scene(assetsManager), m_assetsManager(assetsManager) {
    computeShaders();
    loadCubes();
    m_scene.create(potatoengine::asset::prefab::ID::Player);
}

void GameState::onAttach() {
}

void GameState::onDetach() {
}

void GameState::onUpdate(potatoengine::Time dt) {
    m_cameraController.onUpdate(dt);
    potatoengine::RendererAPI::SetClearColor({0.45f, 0.55f, 0.60f, 1.00f});
    potatoengine::RendererAPI::Clear();

    potatoengine::Renderer::BeginScene(m_cameraController.getCamera());

    (m_assetsManager->get<potatoengine::Texture>(potatoengine::asset::texture::ID::Cube)).get().bind(0);
    for (const auto &cubePosition : cubePositions) {
        glm::mat4 transform = glm::mat4(1.f);
        transform = glm::translate(transform, cubePosition);
        potatoengine::Renderer::Submit(m_basic, m_vao, transform);
    }

    potatoengine::Renderer::EndScene();
}

void GameState::onImGuiRender() {
}

void GameState::onEvent(potatoengine::Event &e) {
    m_cameraController.onEvent(e);

    potatoengine::EventDispatcher dispatcher(e);
    dispatcher.dispatch<potatoengine::KeyPressedEvent>(BIND_EVENT(GameState::onKeyPressed));
    dispatcher.dispatch<potatoengine::KeyReleasedEvent>(BIND_EVENT(GameState::onKeyReleased));
}

bool GameState::onKeyPressed(potatoengine::KeyPressedEvent &e) {
    if (e.repeating())
        return false;

    auto &window = potatoengine::Application::Get().getWindow();
    // bool control = potatoengine::Input::IsKeyPressed(potatoengine::Key::LeftControl) or potatoengine::Input::IsKeyPressed(potatoengine::Key::RightControl);
    // bool shift = potatoengine::Input::IsKeyPressed(potatoengine::Key::LeftShift) or potatoengine::Input::IsKeyPressed(potatoengine::Key::RightShift);

    switch (e.getKeyCode()) {
        case potatoengine::Key::Escape:
            potatoengine::Application::Get().close();
            break;
        case potatoengine::Key::F3:
            m_debugging = not m_debugging;
            break;
        case potatoengine::Key::F4:
            m_wireframe = not m_wireframe;
            potatoengine::RendererAPI::SetWireframe(m_wireframe);
            break;
        case potatoengine::Key::LeftAlt:
            window.setUpdateCameraPosition(false);
            window.setCursorMode(potatoengine::CursorMode::Normal);
            break;
    }

    return true;
}

bool GameState::onKeyReleased(potatoengine::KeyReleasedEvent &e) {
    potatoengine::Window &window = potatoengine::Application::Get().getWindow();

    switch (e.getKeyCode()) {
        case potatoengine::Key::LeftAlt:
            window.setLastMousePosition(potatoengine::Input::GetMouseX(), potatoengine::Input::GetMouseY());
            window.setUpdateCameraPosition(true);
            window.setCursorMode(potatoengine::CursorMode::Disabled);
            break;
    }

    return true;
}

void GameState::computeShaders() {
    CORE_INFO("\tComputing shaders...");
    m_basic = potatoengine::Program::Create("basic");

    const auto &vert = (m_assetsManager->get<potatoengine::Shader>(potatoengine::asset::shader::ID::VBasic)).get();
    const auto &frag = (m_assetsManager->get<potatoengine::Shader>(potatoengine::asset::shader::ID::FBasic)).get();
    potatoengine::Renderer::Link(m_basic, vert, frag);

    m_basic->use();
    m_basic->setMat4("viewProjection", m_cameraController.getCamera().getViewProjection());
    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    m_basic->setInt("mtexture", 0);
    m_basic->setVec4("colorFactor", {0.8f, 0.6f, 0.4f, 1.f});
    m_basic->setVec3("lightDir", {0.5f, 0.5f, 1.f});

    CORE_INFO("\tShaders computed!");
}

void GameState::loadCubes() {  // todo replace with mesh
    CORE_INFO("\tCreating cubes...");
    m_vao = potatoengine::VAO::Create();
    m_vao->bind();

    std::vector<potatoengine::Vertex> vertices = {
        {{-0.5f, -0.5f, -0.5f}, {0.3f, 0.15f, 0.06f}, {0.f, 0.f}},  // A 0
        {{0.5f, -0.5f, -0.5f}, {0.3f, 0.15f, 0.06f}, {1.f, 0.f}},   // B 1
        {{0.5f, 0.5f, -0.5f}, {0.3f, 0.15f, 0.06f}, {1.f, 1.f}},    // C 2
        {{-0.5f, 0.5f, -0.5f}, {0.3f, 0.15f, 0.06f}, {0.f, 1.f}},   // D 3
        {{-0.5f, -0.5f, 0.5f}, {0.3f, 0.15f, 0.06f}, {0.f, 0.f}},   // E 4
        {{0.5f, -0.5f, 0.5f}, {0.3f, 0.15f, 0.06f}, {1.f, 0.f}},    // F 5
        {{0.5f, 0.5f, 0.5f}, {0.3f, 0.15f, 0.06f}, {1.f, 1.f}},     // G 6
        {{-0.5f, 0.5f, 0.5f}, {0.3f, 0.15f, 0.06f}, {0.f, 1.f}},    // H 7

        {{-0.5f, 0.5f, -0.5f}, {0.3f, 0.15f, 0.06f}, {0.f, 0.f}},   // D 8
        {{-0.5f, -0.5f, -0.5f}, {0.3f, 0.15f, 0.06f}, {1.f, 0.f}},  // A 9
        {{-0.5f, -0.5f, 0.5f}, {0.3f, 0.15f, 0.06f}, {1.f, 1.f}},   // E 10
        {{-0.5f, 0.5f, 0.5f}, {0.3f, 0.15f, 0.06f}, {0.f, 1.f}},    // H 11
        {{0.5f, -0.5f, -0.5f}, {0.3f, 0.15f, 0.06f}, {0.f, 0.f}},   // B 12
        {{0.5f, 0.5f, -0.5f}, {0.3f, 0.15f, 0.06f}, {1.f, 0.f}},    // C 13
        {{0.5f, 0.5f, 0.5f}, {0.3f, 0.15f, 0.06f}, {1.f, 1.f}},     // G 14
        {{0.5f, -0.5f, 0.5f}, {0.3f, 0.15f, 0.06f}, {0.f, 1.f}},    // F 15

        {{-0.5f, -0.5f, -0.5f}, {0.3f, 0.15f, 0.06f}, {0.f, 0.f}},  // A 16
        {{0.5f, -0.5f, -0.5f}, {0.3f, 0.15f, 0.06f}, {1.f, 0.f}},   // B 17
        {{0.5f, -0.5f, 0.5f}, {0.3f, 0.15f, 0.06f}, {1.f, 1.f}},    // F 18
        {{-0.5f, -0.5f, 0.5f}, {0.3f, 0.15f, 0.06f}, {0.f, 1.f}},   // E 19
        {{0.5f, 0.5f, -0.5f}, {0.3f, 0.15f, 0.06f}, {0.f, 0.f}},    // C 20
        {{-0.5f, 0.5f, -0.5f}, {0.3f, 0.15f, 0.06f}, {1.f, 0.f}},   // D 21
        {{-0.5f, 0.5f, 0.5f}, {0.3f, 0.15f, 0.06f}, {1.f, 1.f}},    // H 22
        {{0.5f, 0.5f, 0.5f}, {0.3f, 0.15f, 0.06f}, {0.f, 1.f}},     // G 23
    };

    std::vector<uint32_t> indices = {
        // front and back
        0, 3, 2,
        2, 1, 0,
        4, 5, 6,
        6, 7, 4,
        // left and right
        11, 8, 9,
        9, 10, 11,
        12, 13, 14,
        14, 15, 12,
        // bottom and top
        16, 17, 18,
        18, 19, 16,
        20, 21, 22,
        22, 23, 20};

    m_vao->attachVertex(potatoengine::VBO::Create(vertices));
    m_vao->setIndex(potatoengine::IBO::Create(indices));
    CORE_INFO("\tCubes created!");
}

std::unique_ptr<potatoengine::State> GameState::Create(const std::shared_ptr<potatoengine::AssetsManager> &am) {
    return std::make_unique<GameState>(am);
}
}
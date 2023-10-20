#include "demos/states/gameState.h"

#include "demos/settings.h"

namespace potatocraft {

GameState::GameState(std::weak_ptr<engine::AssetsManager> am, std::weak_ptr<engine::Renderer> r) : State("GameState"), m_cameraController(), m_sceneManager(am, r), m_assetsManager(am), m_renderer(r) {
#ifdef DEBUG
    CORE_INFO("Loading scene {}", m_sceneManager.getActiveScene());
#endif
    m_sceneManager.loadScene(DEFAULT_SCENE);
    engine::RendererAPI::SetClearColor(CLEAR_COLOR);
    engine::RendererAPI::SetClearDepth(CLEAR_DEPTH);
}

void GameState::onAttach() {
    m_sceneManager.createScene(DEFAULT_SCENE);
#ifdef DEBUG
    m_sceneManager.print();
#endif
}

void GameState::onDetach() {
#ifdef DEBUG
    CORE_INFO("Detaching GameState");
#endif
    m_sceneManager.clearScene(DEFAULT_SCENE);
}

void GameState::onUpdate(engine::Time ts) {
    m_cameraController.onUpdate(ts);

    const auto& renderer = m_renderer.lock();
    if (not renderer) {
        throw std::runtime_error("Renderer is null");
    }

    renderer->beginScene(m_cameraController.getCamera()); // TODO: move after camera refactor

    m_sceneManager.onUpdate(ts, m_renderer);

    renderer->endScene();
}

void GameState::onImGuiRender() {
}

void GameState::onEvent(engine::Event& e) {
    m_cameraController.onEvent(e);

    engine::EventDispatcher dispatcher(e);
    dispatcher.dispatch<engine::KeyPressedEvent>(BIND_EVENT(GameState::onKeyPressed));
    dispatcher.dispatch<engine::KeyReleasedEvent>(BIND_EVENT(GameState::onKeyReleased));
}

bool GameState::onKeyPressed(engine::KeyPressedEvent& e) {
    if (e.repeating())
        return false;

    auto& window = engine::Application::Get().getWindow();
    // bool control = engine::Input::IsKeyPressed(engine::Key::LeftControl) or engine::Input::IsKeyPressed(engine::Key::RightControl);
    // bool shift = engine::Input::IsKeyPressed(engine::Key::LeftShift) or engine::Input::IsKeyPressed(engine::Key::RightShift);

    switch (e.getKeyCode()) {
        case engine::Key::Escape:
            engine::Application::Get().terminate();
            break;
        case engine::Key::F3:
            m_debugging = not m_debugging;
            break;
        case engine::Key::F4:
            m_wireframe = not m_wireframe;
            engine::RendererAPI::SetWireframe(m_wireframe);
            break;
        case engine::Key::LeftAlt:
            window.setUpdateCameraPosition(false);
            window.setCursorMode(engine::CursorMode::Normal);
            break;
    }

    return true;
}

bool GameState::onKeyReleased(engine::KeyReleasedEvent& e) {
    auto& window = engine::Application::Get().getWindow();

    switch (e.getKeyCode()) {
        case engine::Key::LeftAlt:
            window.setLastMousePosition(engine::Input::GetMouseX(), engine::Input::GetMouseY());
            window.setUpdateCameraPosition(true);
            window.setCursorMode(engine::CursorMode::Disabled);
            break;
    }

    return true;
}

std::unique_ptr<engine::State> GameState::Create(std::weak_ptr<engine::AssetsManager> am, std::weak_ptr<engine::Renderer> r) {
    return std::make_unique<GameState>(am, r);
}
}
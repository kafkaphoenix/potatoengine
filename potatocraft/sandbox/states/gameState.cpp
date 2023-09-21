#include "sandbox/states/gameState.h"

namespace potatocraft {

GameState::GameState(std::weak_ptr<engine::AssetsManager> am, std::weak_ptr<engine::Renderer> r) : State("GameState"), m_cameraController(), m_scene(am), m_assetsManager(am), m_renderer(r) {
    #ifdef DEBUG
        CORE_INFO("Creating prefabs...");
    #endif
    m_scene.createPrefabs();

    const auto& renderer = m_renderer.lock();
    if (not renderer) {
        throw std::runtime_error("Renderer is null");
    }

    renderer->add("basic"); // TODO this should be a gui option
    renderer->add("skybox");
}

void GameState::onAttach() {
    const auto& manager = m_assetsManager.lock();
    if (not manager) {
        throw std::runtime_error("Assets manager is null");
    }

    auto block1 = m_scene.create(engine::assets::PrefabID::GlassBlock); // TODO this should be a gui option
    block1.get<engine::CTransform>().position = {0.f, 0.f, 0.f};
    auto block2 = m_scene.create(engine::assets::PrefabID::GlassBlock); // transparent objects need to be after sky
    block2.get<engine::CTransform>().position = {2.f, 5.f, -15.f};
    auto block10 = m_scene.create(engine::assets::PrefabID::GlassBlock);
    block10.get<engine::CTransform>().position = {-1.3f, 1.f, -1.5f};
    auto skybox = m_scene.create(engine::assets::PrefabID::Skybox); // if depth test false needs to be at the end
    auto block3 = m_scene.create(engine::assets::PrefabID::StoneBlock);
    block3.get<engine::CTransform>().position = {-1.5f, -2.2f, -2.5f};
    auto block4 = m_scene.create(engine::assets::PrefabID::StoneBlock);
    block4.get<engine::CTransform>().position = {-3.8f, -2.f, -12.3f};
    auto block5 = m_scene.create(engine::assets::PrefabID::BrickBlock);
    block5.get<engine::CTransform>().position = {2.4f, -0.4f, -3.5f};
    auto block6 = m_scene.create(engine::assets::PrefabID::BrickBlock);
    block6.get<engine::CTransform>().position = {-1.7f, 3.f, -7.5f};
    auto block7 = m_scene.create(engine::assets::PrefabID::WoodBlock);
    block7.get<engine::CTransform>().position = {1.3f, -2.f, -2.5f};
    auto block8 = m_scene.create(engine::assets::PrefabID::WoodBlock);
    block8.get<engine::CTransform>().position = {1.5f, 2.f, -2.5f};
    auto block9 = m_scene.create(engine::assets::PrefabID::StoneBlock);
    block9.get<engine::CTransform>().position = {1.5f, 0.2f, -1.5f};
    auto sun = m_scene.create(engine::assets::PrefabID::Sun);
#ifdef DEBUG
    m_scene.print();
#endif
}

void GameState::onDetach() {
}

void GameState::onUpdate(engine::Time ts) {
    m_cameraController.onUpdate(ts);
    engine::RendererAPI::SetClearColor({0.45f, 0.55f, 0.6f, 1.f});
    engine::RendererAPI::Clear();

    const auto& renderer = m_renderer.lock();
    if (not renderer) {
        throw std::runtime_error("Renderer is null");
    }
    
    renderer->beginScene(m_cameraController.getCamera());

    m_scene.onUpdate(ts, m_renderer);

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
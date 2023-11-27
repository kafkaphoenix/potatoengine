#include "demos/states/gameState.h"

#include "demos/settings.h"

namespace potatocraft {

GameState::GameState(std::weak_ptr<engine::AssetsManager> am, std::weak_ptr<engine::Renderer> r) : State("GameState"), m_sceneManager(am, r), m_assetsManager(am), m_renderer(r) {
    m_sceneManager.loadScene(DEFAULT_SCENE);
    engine::RendererAPI::SetClearColor(CLEAR_COLOR);
    engine::RendererAPI::SetClearDepth(CLEAR_DEPTH);
}

void GameState::onAttach() {
    m_sceneManager.createScene(DEFAULT_SCENE);
    m_sceneManager.print();
}

void GameState::onDetach() {
    m_sceneManager.clearScene(DEFAULT_SCENE);
}

void GameState::onUpdate(engine::Time ts) {
    m_sceneManager.onUpdate(ts, m_renderer);
}

void GameState::onImGuiRender() {
}

void GameState::onEvent(engine::Event& e) {
    m_sceneManager.onEvent(e);
}

std::unique_ptr<engine::State> GameState::Create(std::weak_ptr<engine::AssetsManager> am, std::weak_ptr<engine::Renderer> r) {
    return std::make_unique<GameState>(am, r);
}
}
#include "states/gameState.h"

#include "serializers/ssettings.h"
#include "ui/imdebugger.h"

namespace demos {

GameState::GameState(std::weak_ptr<engine::AssetsManager> am,
                     std::weak_ptr<engine::Renderer> r, Settings&& s)
  : State("GameState"), m_sceneManager(am, r), m_assetsManager(am),
    m_renderer(r), m_settings(std::move(s)) {
  engine::RendererAPI::SetClearColor(m_settings.clearColor);
  engine::RendererAPI::SetClearDepth(m_settings.clearDepth);
}

void GameState::onAttach() {
  if (not m_settings.activeScene.empty()) {
    const auto& manager = m_assetsManager.lock();
    APP_ASSERT(manager, "AssetsManager is null!");

    manager->load<engine::Scene>(m_settings.activeScene,
                                 m_settings.activeScenePath);
    m_sceneManager.loadScene(m_settings.activeScene);
    m_sceneManager.createScene(m_settings.activeScene);
  }
}

void GameState::onDetach() {
  APP_INFO("Saving settings...");
  serializers::save_settings(m_settings);

  m_sceneManager.clearScene();

  const auto& manager = m_assetsManager.lock();
  if (not manager) {
    APP_WARN("AssetsManager is null!");
  } else {
    manager->clear();
  }
}

void GameState::onUpdate(engine::Time ts) {
  if (m_settings.reloadScene and
      m_sceneManager.getActiveScene() not_eq m_settings.activeScene) {
    const auto& manager = m_assetsManager.lock();
    APP_ASSERT(manager, "AssetsManager is null!");

    manager->clear();
    manager->load<engine::Scene>(m_settings.activeScene,
                                 m_settings.activeScenePath);
    m_sceneManager.clearScene();
    m_sceneManager.loadScene(m_settings.activeScene);
    m_sceneManager.createScene(m_settings.activeScene);
    m_settings.reloadScene = false;
  } else if (m_settings.reloadScene) {
    m_sceneManager.reloadScene();
    m_settings.reloadScene = false;
  }
  m_sceneManager.onUpdate(ts, m_renderer);
}

void GameState::onImguiUpdate() {
  drawDebugger(m_assetsManager, m_renderer, m_sceneManager, m_settings);
}

void GameState::onEvent(engine::Event& e) { m_sceneManager.onEvent(e); }

std::unique_ptr<engine::State>
GameState::Create(std::weak_ptr<engine::AssetsManager> am,
                  std::weak_ptr<engine::Renderer> r, Settings&& s) {
  return std::make_unique<GameState>(am, r, std::move(s));
}
}
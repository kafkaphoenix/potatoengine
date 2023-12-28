#include "states/gameState.h"

#include "serializers/ssettings.h"

namespace demos {

GameState::GameState(std::weak_ptr<engine::AssetsManager> am,
                     std::weak_ptr<engine::Renderer> r,
                     std::weak_ptr<engine::Settings> s,
                     std::weak_ptr<engine::SceneManager> sm)
  : State("GameState"), m_assetsManager(am), m_renderer(r), m_settings(s),
    m_sceneManager(sm) {
  const auto& settings = m_settings.lock();
  APP_ASSERT(settings, "Settings is null!");

  engine::RendererAPI::SetClearColor(settings->clearColor);
  engine::RendererAPI::SetClearDepth(settings->clearDepth);
}

void GameState::onAttach() {
  const auto& settings = m_settings.lock();
  APP_ASSERT(settings, "Settings is null!");

  if (not settings->activeScene.empty()) {
    const auto& manager = m_assetsManager.lock();
    APP_ASSERT(manager, "AssetsManager is null!");

    manager->load<engine::Scene>(settings->activeScene,
                                 settings->activeScenePath);

    const auto& scene_manager = m_sceneManager.lock();
    APP_ASSERT(scene_manager, "SceneManager is null!");

    scene_manager->loadScene(settings->activeScene);
    scene_manager->createScene(settings->activeScene);
  }
}

void GameState::onDetach() {
  const auto& settings = m_settings.lock();
  APP_ASSERT(settings, "Settings is null!");

  APP_INFO("Saving settings...");
  engine::serializers::save_settings(
    settings, engine::serializers::get_default_roaming_path("Demos"));

  const auto& scene_manager = m_sceneManager.lock();
  APP_ASSERT(scene_manager, "SceneManager is null!");

  scene_manager->clearScene();

  const auto& manager = m_assetsManager.lock();
  if (not manager) {
    APP_WARN("AssetsManager is null!");
  } else {
    manager->clear();
  }
}

void GameState::onUpdate(engine::Time ts) {
  const auto& settings = m_settings.lock();
  APP_ASSERT(settings, "Settings is null!");

  const auto& scene_manager = m_sceneManager.lock();
  APP_ASSERT(scene_manager, "SceneManager is null!");

  if (settings->reloadScene and
      scene_manager->getActiveScene() not_eq settings->activeScene) {
    const auto& manager = m_assetsManager.lock();
    APP_ASSERT(manager, "AssetsManager is null!");

    manager->clear();
    manager->load<engine::Scene>(settings->activeScene,
                                 settings->activeScenePath);
    scene_manager->clearScene();
    scene_manager->loadScene(settings->activeScene);
    scene_manager->createScene(settings->activeScene);
    settings->reloadScene = false;
  } else if (settings->reloadScene) {
    scene_manager->reloadScene();
    settings->reloadScene = false;
  }
  scene_manager->onUpdate(ts, m_renderer);
}

void GameState::onImguiUpdate() {}

void GameState::onEvent(engine::Event& e) {
  const auto& scene_manager = m_sceneManager.lock();
  APP_ASSERT(scene_manager, "SceneManager is null!");

  scene_manager->onEvent(e);
}

std::unique_ptr<engine::State> GameState::Create(
  std::weak_ptr<engine::AssetsManager> am, std::weak_ptr<engine::Renderer> r,
  std::weak_ptr<engine::Settings> s, std::weak_ptr<engine::SceneManager> sm) {
  return std::make_unique<GameState>(am, r, s, sm);
}
}
#include "states/gameState.h"

#include "serializers/ssettings.h"

namespace demos {

GameState::GameState() : State("GameState") {}

void GameState::onAttach() {
  auto& app = engine::Application::Get();
  const auto& settings = app.getSettings();
  const auto& scene_manager = app.getSceneManager();
  const auto& asset_manager = app.getAssetsManager();
  const auto& renderer = app.getRenderer();
  engine::RendererAPI::SetClearColor(settings->clearColor);
  engine::RendererAPI::SetClearDepth(settings->clearDepth);

  // TODO revisit this with multiple states
  if (not settings->activeScene.empty()) {
    asset_manager->load<engine::Scene>(settings->activeScene,
                                       settings->activeScenePath);
    scene_manager->loadScene(settings->activeScene, asset_manager);
    scene_manager->createScene(settings->activeScene, asset_manager, renderer,
                               false);
  }
}

void GameState::onDetach() {
  auto& app = engine::Application::Get();
  const auto& settings = app.getSettings();
  const auto& scene_manager = app.getSceneManager();
  const auto& asset_manager = app.getAssetsManager();
  const auto& renderer = app.getRenderer();

  APP_INFO("Saving settings...");
  engine::serializers::save_settings(
    settings, engine::serializers::get_default_roaming_path("Demos"));
  scene_manager->clearScene(renderer);
  asset_manager->clear();
}

void GameState::onUpdate(engine::Time ts) {
  auto& app = engine::Application::Get();
  const auto& settings = app.getSettings();
  const auto& scene_manager = app.getSceneManager();
  const auto& asset_manager = app.getAssetsManager();
  const auto& renderer = app.getRenderer();

  if (settings->reloadScene and
      scene_manager->getActiveScene() not_eq settings->activeScene) {
    asset_manager->clear();
    asset_manager->load<engine::Scene>(settings->activeScene,
                                       settings->activeScenePath);
    scene_manager->clearScene(renderer);
    scene_manager->loadScene(settings->activeScene, asset_manager);
    scene_manager->createScene(settings->activeScene, asset_manager, renderer,
                               false);
    settings->reloadScene = false;
  } else if (settings->reloadScene) {
    scene_manager->reloadScene(asset_manager, renderer);
    settings->reloadScene = false;
  }

  scene_manager->onUpdate(ts);
}

void GameState::onImguiUpdate() {}

void GameState::onEvent(engine::Event& e) {
  const auto& scene_manager = engine::Application::Get().getSceneManager();
  scene_manager->onEvent(e);
}

std::unique_ptr<engine::State> GameState::Create() {
  return std::make_unique<GameState>();
}
}
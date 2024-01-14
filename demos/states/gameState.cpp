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
    asset_manager->load<engine::assets::Scene>(settings->activeScene,
                                       settings->activeScenePath);
    scene_manager->loadScene(settings->activeScene, asset_manager);
    scene_manager->createScene(settings->activeScene, asset_manager, renderer,
                               false);
  }

  if (settings->activeScene == "Flappy Bird") {
    auto& registry = scene_manager->getRegistry();
    auto pipe = scene_manager->getEntity("green_pipe_top");
    auto& ptransform = registry.get<engine::CTransform>(pipe);
    ptransform.position = {1.5f, -0.1f, 0.0f};
    auto& ptexture = registry.get<engine::CShaderProgram>(pipe); // rename
    ptexture.isVisible = true;

    auto pipe2 = scene_manager->getEntity("green_pipe_bottom");
    auto& ptransform2 = registry.get<engine::CTransform>(pipe2);
    ptransform2.position = {1.f, 1.f, 0.0f};
    auto& ptexture2 = registry.get<engine::CShaderProgram>(pipe2);
    ptexture2.isVisible = true;

    auto pipe3 = scene_manager->getEntity("red_pipe_top");
    auto& ptransform3 = registry.get<engine::CTransform>(pipe3);
    ptransform3.position = {-0.5f, -0.1f, 0.0f};
    auto& ptexture3 = registry.get<engine::CShaderProgram>(pipe3);
    ptexture3.isVisible = true;

    auto pipe4 = scene_manager->getEntity("red_pipe_bottom");
    auto& ptransform4 = registry.get<engine::CTransform>(pipe4);
    ptransform4.position = {-1.5f, 0.75f, 0.0f};
    auto& ptexture4 = registry.get<engine::CShaderProgram>(pipe4);
    ptexture4.isVisible = true;
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
    asset_manager->load<engine::assets::Scene>(settings->activeScene,
                                       settings->activeScenePath);
    scene_manager->clearScene(renderer);
    scene_manager->loadScene(settings->activeScene, asset_manager);
    scene_manager->createScene(settings->activeScene, asset_manager, renderer,
                               false);
    settings->reloadScene = false;
    if (settings->activeScene == "Flappy Bird") { // TODO remove this
      auto& registry = scene_manager->getRegistry();
      auto pipe = scene_manager->getEntity("green_pipe_top");
      auto& ptransform = registry.get<engine::CTransform>(pipe);
      ptransform.position = {1.5f, -0.1f, 0.0f};
      auto& ptexture = registry.get<engine::CShaderProgram>(pipe); // rename
      ptexture.isVisible = true;

      auto pipe2 = scene_manager->getEntity("green_pipe_bottom");
      auto& ptransform2 = registry.get<engine::CTransform>(pipe2);
      ptransform2.position = {1.f, 1.f, 0.0f};
      auto& ptexture2 = registry.get<engine::CShaderProgram>(pipe2);
      ptexture2.isVisible = true;

      auto pipe3 = scene_manager->getEntity("red_pipe_top");
      auto& ptransform3 = registry.get<engine::CTransform>(pipe3);
      ptransform3.position = {-0.5f, -0.1f, 0.0f};
      auto& ptexture3 = registry.get<engine::CShaderProgram>(pipe3);
      ptexture3.isVisible = true;

      auto pipe4 = scene_manager->getEntity("red_pipe_bottom");
      auto& ptransform4 = registry.get<engine::CTransform>(pipe4);
      ptransform4.position = {-1.5f, 0.75f, 0.0f};
      auto& ptexture4 = registry.get<engine::CShaderProgram>(pipe4);
      ptexture4.isVisible = true;
    }
  } else if (settings->reloadScene) {
    scene_manager->reloadScene(asset_manager, renderer);
    settings->reloadScene = false;
    if (settings->activeScene == "Flappy Bird") { // TODO remove this
      auto& registry = scene_manager->getRegistry();
      auto pipe = scene_manager->getEntity("green_pipe_top");
      auto& ptransform = registry.get<engine::CTransform>(pipe);
      ptransform.position = {1.5f, -0.1f, 0.0f};
      auto& ptexture = registry.get<engine::CShaderProgram>(pipe); // rename
      ptexture.isVisible = true;

      auto pipe2 = scene_manager->getEntity("green_pipe_bottom");
      auto& ptransform2 = registry.get<engine::CTransform>(pipe2);
      ptransform2.position = {1.f, 1.f, 0.0f};
      auto& ptexture2 = registry.get<engine::CShaderProgram>(pipe2);
      ptexture2.isVisible = true;

      auto pipe3 = scene_manager->getEntity("red_pipe_top");
      auto& ptransform3 = registry.get<engine::CTransform>(pipe3);
      ptransform3.position = {-0.5f, -0.1f, 0.0f};
      auto& ptexture3 = registry.get<engine::CShaderProgram>(pipe3);
      ptexture3.isVisible = true;

      auto pipe4 = scene_manager->getEntity("red_pipe_bottom");
      auto& ptransform4 = registry.get<engine::CTransform>(pipe4);
      ptransform4.position = {-1.5f, 0.75f, 0.0f};
      auto& ptexture4 = registry.get<engine::CShaderProgram>(pipe4);
      ptexture4.isVisible = true;
    }
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
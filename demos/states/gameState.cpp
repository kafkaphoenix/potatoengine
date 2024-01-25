#include "states/gameState.h"

#include "serializers/ssettings.h"
#include "systems/animation/sAnimation.h"
#include "systems/core/sDelete.h"
#include "systems/graphics/sRender.h"
#include "systems/physics/sCollision.h"
#include "systems/physics/sGravity.h"
#include "systems/physics/sMovement.h"
#include "systems/terrain/sPipes.h"
#include "systems/terrain/sTerrain.h"
#include "systems/world/sLight.h"
#include "systems/world/sSkybox.h"
#include "systems/world/sTime.h"
#include "dispatchers/appDispatcher.h"
#include "dispatchers/inputDispatcher.h"

namespace demos {

GameState::GameState() : State("GameState") {}

void GameState::onAttach() {
  auto& app = engine::Application::Get();
  const auto& settings = app.getSettings();
  const auto& scene_manager = app.getSceneManager();
  scene_manager->registerSystem(std::make_unique<systems::DeleteSystem>(-100));
  scene_manager->registerSystem(std::make_unique<systems::TimeSystem>(0));
  scene_manager->registerSystem(std::make_unique<systems::TerrainSystem>(1));
  scene_manager->registerSystem(std::make_unique<systems::PipesSystem>(2));
  scene_manager->registerSystem(std::make_unique<systems::SkyboxSystem>(3));
  scene_manager->registerSystem(std::make_unique<systems::LightSystem>(4));
  scene_manager->registerSystem(std::make_unique<systems::CollisionSystem>(5));
  scene_manager->registerSystem(std::make_unique<systems::GravitySystem>(6));
  scene_manager->registerSystem(std::make_unique<systems::AnimationSystem>(7));
  scene_manager->registerSystem(std::make_unique<systems::MovementSystem>(8));
  scene_manager->registerSystem(std::make_unique<systems::RenderSystem>(100));
  engine::RendererAPI::SetClearColor(settings->clearColor);
  engine::RendererAPI::SetClearDepth(settings->clearDepth);

  scene_manager->createScene(settings->activeScene, settings->activeScenePath);

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

  APP_INFO("Saving settings...");
  engine::serializers::save_settings(
    settings, engine::serializers::get_default_roaming_path("Demos"));
  scene_manager->clearScene();
  asset_manager->clear();
}

void GameState::onUpdate(const engine::Time& ts) {
  auto& app = engine::Application::Get();
  const auto& settings = app.getSettings();
  const auto& scene_manager = app.getSceneManager();
  const auto& asset_manager = app.getAssetsManager();

  if (settings->reloadScene) {
    if (scene_manager->getActiveScene() == settings->activeScene) {
      scene_manager->reloadScene(false);
    } else {
      scene_manager->clearScene();
      asset_manager->clear();
      scene_manager->createScene(settings->activeScene,
                                 settings->activeScenePath);
    }
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

void GameState::onEvent(engine::events::Event& e) {
  auto& registry = engine::Application::Get().getSceneManager()->getRegistry();
  
  dispatchers::appDispatcher(registry, e);
  dispatchers::inputDispatcher(registry, e);
}

std::unique_ptr<engine::State> GameState::Create() {
  return std::make_unique<GameState>();
}
}
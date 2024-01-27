#include "states/gameState.h"

#include "components/core/cState.h"
#include "dispatchers/appDispatcher.h"
#include "dispatchers/inputDispatcher.h"
#include "serializers/ssettings.h"
#include "states/scoreState.h"
#include "systems/animation/sAnimation.h"
#include "systems/core/sDelete.h"
#include "systems/core/sTime.h"
#include "systems/graphics/sRender.h"
#include "systems/items/sCoins.h"
#include "systems/meta/sScore.h"
#include "systems/meta/sTimer.h"
#include "systems/physics/sCollision.h"
#include "systems/physics/sGravity.h"
#include "systems/physics/sMovement.h"
#include "systems/terrain/sPipes.h"
#include "systems/terrain/sTerrain.h"
#include "systems/world/sLight.h"
#include "systems/world/sSkybox.h"

namespace demos::states {

GameState::GameState() : State("GameState") {}

void GameState::onAttach() {
  auto& app = engine::Application::Get();
  const auto& settings = app.getSettings();
  const auto& scene_manager = app.getSceneManager();
  scene_manager->registerSystem("delete_system",
                                std::make_unique<systems::DeleteSystem>(-100));
  scene_manager->registerSystem("render_system",
                                std::make_unique<systems::RenderSystem>(100));

  if (settings->activeScene == "Flappy Bird") {
    scene_manager->registerSystem("time_system",
                                  std::make_unique<systems::TimeSystem>(0));
    scene_manager->registerSystem(
      "animation_system", std::make_unique<systems::AnimationSystem>(7));
    // TODO background system 3
    auto& registry = scene_manager->getRegistry();
    auto bird = scene_manager->getEntity("bird");
    registry.get<engine::CShaderProgram>(bird).isVisible = true;
    registry.get<engine::CTransform>(bird).position.y = 0.2;

    auto getReady = scene_manager->getEntity("ready");
    registry.get<engine::CShaderProgram>(getReady).isVisible = true;

    auto countdown =
      scene_manager->createEntity("scene", "numbers", "countdown");
    registry.get<engine::CTextureAtlas>(countdown).index = 5;
    registry.get<engine::CTransform>(countdown).position.y = 0.45;

    auto gamestate = scene_manager->getEntity("gamestate");
    registry.get<CState>(gamestate).state = CState::State::STARTING;
  } else if (settings->activeScene == "Plane terrain") {
    scene_manager->createScene(settings->activeScene,
                               settings->activeScenePath);
    scene_manager->registerSystem("time_system",
                                  std::make_unique<systems::TimeSystem>(0));
    scene_manager->registerSystem("terrain_system",
                                  std::make_unique<systems::TerrainSystem>(1));
    scene_manager->registerSystem("skybox_system",
                                  std::make_unique<systems::SkyboxSystem>(3));
    scene_manager->registerSystem("movement_system",
                                  std::make_unique<systems::MovementSystem>(8));
  } else if (settings->activeScene == "Cubes") {
    scene_manager->createScene(settings->activeScene,
                               settings->activeScenePath);
    scene_manager->registerSystem("time_system",
                                  std::make_unique<systems::TimeSystem>(0));
    scene_manager->registerSystem("skybox_system",
                                  std::make_unique<systems::SkyboxSystem>(3));
    scene_manager->registerSystem("light_system",
                                  std::make_unique<systems::LightSystem>(4));
    scene_manager->registerSystem(
      "animation_system", std::make_unique<systems::AnimationSystem>(7));
    scene_manager->registerSystem("movement_system",
                                  std::make_unique<systems::MovementSystem>(8));
  } else if (settings->activeScene == "Skycrapers") {
    scene_manager->createScene(settings->activeScene,
                               settings->activeScenePath);
    scene_manager->registerSystem("time_system",
                                  std::make_unique<systems::TimeSystem>(0));
    scene_manager->registerSystem("skybox_system",
                                  std::make_unique<systems::SkyboxSystem>(3));
    scene_manager->registerSystem("movement_system",
                                  std::make_unique<systems::MovementSystem>(8));
  } else if (settings->activeScene == "Primitives") {
    scene_manager->createScene(settings->activeScene,
                               settings->activeScenePath);
    scene_manager->registerSystem("time_system",
                                  std::make_unique<systems::TimeSystem>(0));
    scene_manager->registerSystem("skybox_system",
                                  std::make_unique<systems::SkyboxSystem>(3));
    scene_manager->registerSystem("movement_system",
                                  std::make_unique<systems::MovementSystem>(8));
  } else {
    scene_manager->createScene(settings->activeScene,
                               settings->activeScenePath);
    scene_manager->registerSystem("movement_system",
                                  std::make_unique<systems::MovementSystem>(8));
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
  auto& registry = scene_manager->getRegistry();

  if (settings->reloadScene) {
    if (scene_manager->getActiveScene() == settings->activeScene) {
      scene_manager->reloadScene(settings->reloadPrototypes);

      if (settings->activeScene == "Flappy Bird") {
        // TODO remove this after moving to different project as
        // we dont need to reload from other demo scenes deleting the
        // templates or we will delete in retry some of them
        // maybe a loading screen state
        registry
          .view<engine::CShaderProgram, engine::CTransform, engine::CName,
                engine::CUUID>()
          .each([&](engine::CShaderProgram& cShaderProgram,
                    engine::CTransform& cTransform, const engine::CName& cName,
                    const engine::CUUID& cUUID) {
            if (cName.name == "ready") {
              cShaderProgram.isVisible = true;
            } else if (cName.name == "title") {
              cShaderProgram.isVisible = false;
            } else if (cName.name == "start") {
              cShaderProgram.isVisible = false;
            } else if (cName.name == "exit") {
              cShaderProgram.isVisible = false;
            }
          });

        auto countdown =
          scene_manager->createEntity("scene", "numbers", "countdown");
        registry.get<engine::CTextureAtlas>(countdown).index = 5;
        registry.get<engine::CTransform>(countdown).position.y = 0.45;

        auto gamestate = scene_manager->getEntity("gamestate");
        registry.get<CState>(gamestate).state = CState::State::STARTING;

        scene_manager->unregisterSystem("pipes_system");
        scene_manager->unregisterSystem("coin_system");
        scene_manager->unregisterSystem("timer_system");
        scene_manager->unregisterSystem("collision_system");
        scene_manager->unregisterSystem("gravity_system");
        scene_manager->unregisterSystem("score_system");
        scene_manager->unregisterSystem("movement_system");
        scene_manager->unregisterSystem("time_system");
        scene_manager->registerSystem("time_system",
                                      std::make_unique<systems::TimeSystem>(0));
      } else if (settings->activeScene == "Plane terrain") {
        scene_manager->unregisterSystem("time_system");
        scene_manager->registerSystem("time_system",
                                      std::make_unique<systems::TimeSystem>(0));
        scene_manager->unregisterSystem("terrain_system");
        scene_manager->registerSystem(
          "terrain_system", std::make_unique<systems::TerrainSystem>(1));
        scene_manager->unregisterSystem("skybox_system");
        scene_manager->registerSystem(
          "skybox_system", std::make_unique<systems::SkyboxSystem>(3));
      } else if (settings->activeScene == "Cubes") {
        scene_manager->unregisterSystem("time_system");
        scene_manager->registerSystem("time_system",
                                      std::make_unique<systems::TimeSystem>(0));
        scene_manager->unregisterSystem("skybox_system");
        scene_manager->registerSystem(
          "skybox_system", std::make_unique<systems::SkyboxSystem>(3));
        scene_manager->unregisterSystem("light_system");
        scene_manager->registerSystem(
          "light_system", std::make_unique<systems::LightSystem>(4));
      } else if (settings->activeScene == "Skycrapers") {
        scene_manager->unregisterSystem("time_system");
        scene_manager->registerSystem("time_system",
                                      std::make_unique<systems::TimeSystem>(0));
        scene_manager->unregisterSystem("skybox_system");
        scene_manager->registerSystem(
          "skybox_system", std::make_unique<systems::SkyboxSystem>(3));
      } else if (settings->activeScene == "Primitives") {
        scene_manager->unregisterSystem("time_system");
        scene_manager->registerSystem("time_system",
                                      std::make_unique<systems::TimeSystem>(0));
        scene_manager->unregisterSystem("skybox_system");
        scene_manager->registerSystem(
          "skybox_system", std::make_unique<systems::SkyboxSystem>(3));
      }
    } else {
      scene_manager->clearScene();
      asset_manager->clear();
      if (settings->activeScene == "Flappy Bird") {
        scene_manager->createScene(settings->activeScene,
                                   settings->activeScenePath);
        // TODO remove this after moving to different project as
        // we dont need to reload from other demo scenes deleting the
        // templates or we will delete in retry some of them
        // maybe a loading screen state
        registry.view<engine::CShaderProgram, engine::CName, engine::CUUID>()
          .each([&](entt::entity e, engine::CShaderProgram& cShaderProgram,
                    const engine::CName& cName, const engine::CUUID& cUUID) {
            if (cName.name == "bird") {
              registry.get<engine::CTransform>(e).position.y = 0.2;
            } else if (cName.name == "title") {
              cShaderProgram.isVisible = false;
            } else if (cName.name == "start") {
              cShaderProgram.isVisible = false;
            } else if (cName.name == "exit") {
              cShaderProgram.isVisible = false;
            }
          });
      }
      onAttach();
    }

    settings->reloadScene = false;
  } else {
    if (settings->activeScene == "Flappy Bird") {
      entt::entity gamestate = scene_manager->getEntity("gamestate");
      CState& state = registry.get<CState>(gamestate);

      if (state.state == CState::State::READY) {
        entt::entity getReady = scene_manager->getEntity("ready");
        registry.get<engine::CShaderProgram>(getReady).isVisible = false;

        scene_manager->registerSystem(
          "pipes_system", std::make_unique<systems::PipesSystem>(1));
        scene_manager->registerSystem(
          "coin_system", std::make_unique<systems::CoinsSystem>(2));
        scene_manager->registerSystem(
          "timer_system", std::make_unique<systems::TimerSystem>(4));
        scene_manager->registerSystem(
          "collision_system", std::make_unique<systems::CollisionSystem>(5));
        scene_manager->registerSystem(
          "gravity_system", std::make_unique<systems::GravitySystem>(6));
        scene_manager->registerSystem(
          "movement_system", std::make_unique<systems::MovementSystem>(8));
        scene_manager->registerSystem(
          "score_system", std::make_unique<systems::ScoreSystem>(9));
        state.state = CState::State::RUNNING;
      } else if (state.state == CState::State::STOPPED) {
        app.pushOverlay(states::ScoreState::Create());
      }
    }
  }

  scene_manager->onUpdate(ts);
}

void GameState::onEvent(engine::events::Event& e) {
  auto& registry = engine::Application::Get().getSceneManager()->getRegistry();

  dispatchers::appDispatcher(registry, e);
  dispatchers::inputDispatcher(registry, e);
}

std::unique_ptr<engine::State> GameState::Create() {
  return std::make_unique<GameState>();
}
}
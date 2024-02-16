#include "states/gameState.h"

#include "dispatchers/common/inputDispatcher.h"
#include "dispatchers/common/windowDispatcher.h"
#include "dispatchers/game/gameAppDispatcher.h"
#include "layers/game/gameLayer.h"
#include "layers/game/gameoverOverlay.h"
#include "layers/game/getReadyOverlay.h"
#include "layers/game/levelCompletedOverlay.h"
#include "layers/game/pauseOverlay.h"
#include "serializers/ssettings.h"
#include "systems/animation/sAnimation.h"
#include "systems/core/sDelete.h"
#include "systems/core/sTime.h"
#include "systems/graphics/sRender.h"
#include "systems/physics/sMovement.h"
#include "systems/terrain/sTerrain.h"
#include "systems/world/sLight.h"
#include "systems/world/sSkybox.h"

namespace demos::states {

GameState::GameState() : State("GameState") {}

void GameState::onAttach() {
  auto& app = engine::Application::Get();
  const auto& settings_manager = app.getSettingsManager();
  const auto& scene_manager = app.getSceneManager();
  scene_manager->registerSystem("delete_system",
                                std::make_unique<systems::DeleteSystem>(-100));
  scene_manager->registerSystem("render_system",
                                std::make_unique<systems::RenderSystem>(100));

  if (settings_manager->activeScene == "Flappy Bird") {
    const auto& states_manager = engine::Application::Get().getStatesManager();
    states_manager->pushLayer(layers::GameLayer::Create());
    states_manager->pushOverlay(layers::GetReadyOverlay::Create(), true);
    states_manager->pushOverlay(layers::PauseOverlay::Create(), false);
    states_manager->pushOverlay(layers::LevelCompletedOverlay::Create(), false);
    states_manager->pushOverlay(layers::GameoverOverlay::Create(), false);
    scene_manager->registerSystem("time_system",
                                  std::make_unique<systems::TimeSystem>(0));
    scene_manager->registerSystem(
      "animation_system", std::make_unique<systems::AnimationSystem>(7));
    // TODO background system 3
  } else if (settings_manager->activeScene == "Plane terrain") {
    scene_manager->createScene(settings_manager->activeScene,
                               settings_manager->activeScenePath);
    scene_manager->registerSystem("time_system",
                                  std::make_unique<systems::TimeSystem>(0));
    scene_manager->registerSystem("terrain_system",
                                  std::make_unique<systems::TerrainSystem>(1));
    scene_manager->registerSystem("skybox_system",
                                  std::make_unique<systems::SkyboxSystem>(3));
    scene_manager->registerSystem("movement_system",
                                  std::make_unique<systems::MovementSystem>(8));
  } else if (settings_manager->activeScene == "Cubes") {
    scene_manager->createScene(settings_manager->activeScene,
                               settings_manager->activeScenePath);
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
  } else if (settings_manager->activeScene == "Skycrapers") {
    scene_manager->createScene(settings_manager->activeScene,
                               settings_manager->activeScenePath);
    scene_manager->registerSystem("time_system",
                                  std::make_unique<systems::TimeSystem>(0));
    scene_manager->registerSystem("skybox_system",
                                  std::make_unique<systems::SkyboxSystem>(3));
    scene_manager->registerSystem("movement_system",
                                  std::make_unique<systems::MovementSystem>(8));
  } else if (settings_manager->activeScene == "Primitives") {
    scene_manager->createScene(settings_manager->activeScene,
                               settings_manager->activeScenePath);
    scene_manager->registerSystem("time_system",
                                  std::make_unique<systems::TimeSystem>(0));
    scene_manager->registerSystem("skybox_system",
                                  std::make_unique<systems::SkyboxSystem>(3));
    scene_manager->registerSystem("movement_system",
                                  std::make_unique<systems::MovementSystem>(8));
  } else {
    scene_manager->createScene(settings_manager->activeScene,
                               settings_manager->activeScenePath);
    scene_manager->registerSystem("movement_system",
                                  std::make_unique<systems::MovementSystem>(8));
  }
}

void GameState::onDetach() {
  auto& app = engine::Application::Get();
  const auto& settings_manager = app.getSettingsManager();
  const auto& scene_manager = app.getSceneManager();
  const auto& asset_manager = app.getAssetsManager();

  APP_INFO("Saving settings...");
  engine::serializers::save_settings(settings_manager,
                                     engine::get_default_roaming_path("Demos"));

  // TODO rethink these 3 methods
  if (settings_manager->activeScene not_eq "Flappy Bird") {
    scene_manager->clearScene();
    asset_manager->clear();
  } else {
    scene_manager->clearSystems();
    // clean entities from systems and time
  }
}

void GameState::onUpdate(const engine::Time& ts) {
  auto& app = engine::Application::Get();
  const auto& settings_manager = app.getSettingsManager();
  const auto& scene_manager = app.getSceneManager();

  if (settings_manager->reloadScene) {
    if (scene_manager->getActiveScene() == settings_manager->activeScene) {
      if (settings_manager->activeScene == "Flappy Bird") {
        // TODO remove this, we reload in game layer
      } else if (settings_manager->activeScene == "Plane terrain") {
        scene_manager->reloadScene(settings_manager->reloadPrototypes);

        scene_manager->unregisterSystem("time_system");
        scene_manager->registerSystem("time_system",
                                      std::make_unique<systems::TimeSystem>(0));
        scene_manager->unregisterSystem("terrain_system");
        scene_manager->registerSystem(
          "terrain_system", std::make_unique<systems::TerrainSystem>(1));
        scene_manager->unregisterSystem("skybox_system");
        scene_manager->registerSystem(
          "skybox_system", std::make_unique<systems::SkyboxSystem>(3));
        settings_manager->reloadScene = false;
      } else if (settings_manager->activeScene == "Cubes") {
        scene_manager->reloadScene(settings_manager->reloadPrototypes);

        scene_manager->unregisterSystem("time_system");
        scene_manager->registerSystem("time_system",
                                      std::make_unique<systems::TimeSystem>(0));
        scene_manager->unregisterSystem("skybox_system");
        scene_manager->registerSystem(
          "skybox_system", std::make_unique<systems::SkyboxSystem>(3));
        scene_manager->unregisterSystem("light_system");
        scene_manager->registerSystem(
          "light_system", std::make_unique<systems::LightSystem>(4));
        settings_manager->reloadScene = false;
      } else if (settings_manager->activeScene == "Skycrapers") {
        scene_manager->reloadScene(settings_manager->reloadPrototypes);

        scene_manager->unregisterSystem("time_system");
        scene_manager->registerSystem("time_system",
                                      std::make_unique<systems::TimeSystem>(0));
        scene_manager->unregisterSystem("skybox_system");
        scene_manager->registerSystem(
          "skybox_system", std::make_unique<systems::SkyboxSystem>(3));
        settings_manager->reloadScene = false;
      } else if (settings_manager->activeScene == "Primitives") {
        scene_manager->reloadScene(settings_manager->reloadPrototypes);

        scene_manager->unregisterSystem("time_system");
        scene_manager->registerSystem("time_system",
                                      std::make_unique<systems::TimeSystem>(0));
        scene_manager->unregisterSystem("skybox_system");
        scene_manager->registerSystem(
          "skybox_system", std::make_unique<systems::SkyboxSystem>(3));
        settings_manager->reloadScene = false;
      } else {
        scene_manager->reloadScene(settings_manager->reloadPrototypes);
        settings_manager->reloadScene = false;
      }
    } else {
      app.getStatesManager()->getCurrentState()->clearLayers();
      scene_manager->clearScene();
      app.getAssetsManager()->clear();
      if (settings_manager->activeScene == "Flappy Bird") {
        scene_manager->createScene(settings_manager->activeScene,
                                   settings_manager->activeScenePath);
      }
      settings_manager->reloadScene = false;
      onAttach();
    }
  }

  if (settings_manager->activeScene == "Flappy Bird") {
    for (auto it = m_layersManager->begin(); it not_eq m_layersManager->end();
         ++it) {
      if ((*it)->isEnabled()) {
        (*it)->onUpdate(ts);
      }
    }
  }
}

void GameState::onEvent(engine::events::Event& e) {
  auto& app = engine::Application::Get();
  const auto& settings_manager = app.getSettingsManager();
  auto& registry = app.getSceneManager()->getRegistry();

  dispatchers::windowDispatcher(e, registry);
  dispatchers::inputDispatcher(e, registry);

  if (settings_manager->activeScene == "Flappy Bird") {
    dispatchers::gameAppDispatcher(e, registry);

    if (e.isHandled) {
      return;
    }

    for (auto it = m_layersManager->rbegin(); it not_eq m_layersManager->rend();
         ++it) {
      if ((*it)->isEnabled()) {
        (*it)->onEvent(e);
      }
      // to avoid event poping state invalidating the layers iterator
      if (e.isHandled) {
        break;
      }
    }
  }
}

std::unique_ptr<engine::State> GameState::Create() {
  return std::make_unique<GameState>();
}
}
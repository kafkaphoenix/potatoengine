#include "layers/game/gameLayer.h"

#include "systems/core/sTime.h"

namespace demos::layers {

GameLayer::GameLayer() : Layer("GameLayer") {}

void GameLayer::onDetach() {
  auto& app = engine::Application::Get();
  const auto& scene_manager = app.getSceneManager();

  auto gamestate = scene_manager->getEntity("gamestate");
  scene_manager->removeEntity(gamestate);
}

void GameLayer::onUpdate(const engine::Time& ts) {
  auto& app = engine::Application::Get();
  const auto& settings_manager = app.getSettingsManager();
  if (settings_manager->reloadScene) {
    const auto& scene_manager = app.getSceneManager();
    scene_manager->reloadScene(settings_manager->reloadPrototypes);
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
    app.getStatesManager()->enableOverlay("GetReadyOverlay");
    settings_manager->reloadScene = false;
  }
}

void GameLayer::onEvent(engine::events::Event& e) {
  engine::events::EventDispatcher dispatcher(e);
  dispatcher.dispatch<engine::events::KeyPressedEvent>(
    BIND_EVENT(onKeyPressed));
}

bool GameLayer::onKeyPressed(engine::events::KeyPressedEvent& e) {
  // repeat event if key is held down
  if (e.repeating())
    return false;

  if (e.getKeyCode() == engine::Key::R) {
    engine::Application::Get().getSettingsManager()->reloadScene = true;
    return true;
  } else if (e.getKeyCode() == engine::Key::P) {
    engine::Application::Get().getStatesManager()->enableOverlay("PauseOverlay");
    return true;
  }

  return false;
}

std::unique_ptr<engine::Layer> GameLayer::Create() {
  return std::make_unique<GameLayer>();
}
}
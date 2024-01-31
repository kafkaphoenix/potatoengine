#include "layers/game/getReadyOverlay.h"

#include "systems/items/sCoins.h"
#include "systems/meta/sScore.h"
#include "systems/meta/sTimer.h"
#include "systems/physics/sCollision.h"
#include "systems/physics/sGravity.h"
#include "systems/physics/sMovement.h"
#include "systems/terrain/sPipes.h"

namespace demos::layers {

GetReadyOverlay::GetReadyOverlay() : Layer("GetReadyOverlay") {}

void GetReadyOverlay::onAttach() {
  auto& app = engine::Application::Get();
  const auto& scene_manager = app.getSceneManager();
  auto& registry = app.getSceneManager()->getRegistry();

  auto bird = scene_manager->getEntity("bird");
  registry.get<engine::CShaderProgram>(bird).isVisible = true;
  registry.get<engine::CTransform>(bird).position.y = 0.2;

  auto getReady = scene_manager->getEntity("ready");
  registry.get<engine::CShaderProgram>(getReady).isVisible = true;

  auto countdown = scene_manager->createEntity("scene", "numbers", "countdown");
  registry.get<engine::CTextureAtlas>(countdown).index = 3;
  registry.get<engine::CTransform>(countdown).position.y = 0.45;

  scene_manager->createEntity("meta", "gamestate", "gamestate");

  app.getRenderManager()->reorder();
}

void GetReadyOverlay::onDetach() {
  auto& app = engine::Application::Get();
  const auto& scene_manager = app.getSceneManager();
  auto& registry = app.getSceneManager()->getRegistry();
  entt::entity getReady = scene_manager->getEntity("ready");
  registry.get<engine::CShaderProgram>(getReady).isVisible = false;
  entt::entity countdown = scene_manager->getEntity("countdown");
  scene_manager->removeEntity(countdown);

  scene_manager->registerSystem("pipes_system",
                                std::make_unique<systems::PipesSystem>(1));
  scene_manager->registerSystem("coin_system",
                                std::make_unique<systems::CoinsSystem>(2));
  scene_manager->registerSystem("timer_system",
                                std::make_unique<systems::TimerSystem>(4));
  scene_manager->registerSystem("collision_system",
                                std::make_unique<systems::CollisionSystem>(5));
  scene_manager->registerSystem("gravity_system",
                                std::make_unique<systems::GravitySystem>(6));
  scene_manager->registerSystem("movement_system",
                                std::make_unique<systems::MovementSystem>(8));
  scene_manager->registerSystem("score_system",
                                std::make_unique<systems::ScoreSystem>(9));
}

void GetReadyOverlay::onEvent(engine::events::Event& e) {
  engine::events::EventDispatcher dispatcher(e);
  dispatcher.dispatch<engine::events::KeyPressedEvent>(
    BIND_EVENT(onKeyPressed));
}

bool GetReadyOverlay::onKeyPressed(engine::events::KeyPressedEvent& e) {
  // repeat event if key is held down
  if (e.repeating())
    return false;

  if (e.getKeyCode() == engine::Key::R) {
    return true;
  } else if (e.getKeyCode() == engine::Key::P) {
    return true;
  }

  return false;
}

std::unique_ptr<engine::Layer> GetReadyOverlay::Create() {
  return std::make_unique<GetReadyOverlay>();
}
}
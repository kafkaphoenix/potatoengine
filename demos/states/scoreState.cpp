#include "states/scoreState.h"

#include "components/core/cState.h"
#include "dispatchers/appDispatcher.h"
#include "dispatchers/inputDispatcher.h"

namespace demos::states {

ScoreState::ScoreState() : State("ScoreState") {}

void ScoreState::onAttach() {
  auto& app = engine::Application::Get();
  const auto& scene_manager = app.getSceneManager();
  auto& registry = app.getSceneManager()->getRegistry();

  entt::entity gamestate = scene_manager->getEntity("gamestate");
  CState& state = registry.get<CState>(gamestate);

  if (state.state == CState::State::STOPPED) {
    auto completed = scene_manager->getEntity("completed");
    registry.get<engine::CShaderProgram>(completed).isVisible = true;
  } else if (state.state == CState::State::GAMEOVER) {
    auto gameover = scene_manager->getEntity("gameover");
    registry.get<engine::CShaderProgram>(gameover).isVisible = true;
  }

  auto restart = scene_manager->getEntity("restart");
  registry.get<engine::CShaderProgram>(restart).isVisible = true;

  auto menu = scene_manager->getEntity("menu");
  registry.get<engine::CShaderProgram>(menu).isVisible = true;

  app.togglePauseGame(true);
}

void ScoreState::onDetach() {
  auto& app = engine::Application::Get();
  const auto& scene_manager = app.getSceneManager();
  auto& registry = app.getSceneManager()->getRegistry();

  entt::entity gamestate = scene_manager->getEntity("gamestate");
  CState& state = registry.get<CState>(gamestate);

  if (state.state == CState::State::STOPPED) {
    auto completed = scene_manager->getEntity("completed");
    registry.get<engine::CShaderProgram>(completed).isVisible = false;
  } else if (state.state == CState::State::GAMEOVER) {
    auto gameover = scene_manager->getEntity("gameover");
    registry.get<engine::CShaderProgram>(gameover).isVisible = false;
  }

  auto restart = scene_manager->getEntity("restart");
  registry.get<engine::CShaderProgram>(restart).isVisible = false;

  auto menu = scene_manager->getEntity("menu");
  registry.get<engine::CShaderProgram>(menu).isVisible = false;

  app.togglePauseGame(false);
}

void ScoreState::onUpdate(const engine::Time& ts) {
  engine::Application::Get().getSceneManager()->onUpdate(ts);
}

void ScoreState::onEvent(engine::events::Event& e) {
  auto& registry = engine::Application::Get().getSceneManager()->getRegistry();

  dispatchers::appDispatcher(registry, e);
  dispatchers::inputDispatcher(registry, e);
}

std::unique_ptr<engine::State> ScoreState::Create() {
  return std::make_unique<ScoreState>();
}
}
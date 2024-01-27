#include "states/pauseState.h"

#include "dispatchers/appDispatcher.h"
#include "dispatchers/inputDispatcher.h"

namespace demos::states {

PauseState::PauseState() : State("PauseState") {}

void PauseState::onAttach() {
  auto& app = engine::Application::Get();
  const auto& scene_manager = app.getSceneManager();
  auto& registry = app.getSceneManager()->getRegistry();

  auto paused = scene_manager->getEntity("paused");
  registry.get<engine::CShaderProgram>(paused).isVisible = true;

  auto resume = scene_manager->getEntity("resume");
  registry.get<engine::CShaderProgram>(resume).isVisible = true;

  auto restart = scene_manager->getEntity("restart");
  registry.get<engine::CShaderProgram>(restart).isVisible = true;

  auto menu = scene_manager->getEntity("menu");
  registry.get<engine::CShaderProgram>(menu).isVisible = true;

  app.togglePauseGame(true);
}

void PauseState::onDetach() {
  auto& app = engine::Application::Get();
  const auto& scene_manager = app.getSceneManager();
  auto& registry = app.getSceneManager()->getRegistry();

  auto paused = scene_manager->getEntity("paused");
  registry.get<engine::CShaderProgram>(paused).isVisible = false;

  auto resume = scene_manager->getEntity("resume");
  registry.get<engine::CShaderProgram>(resume).isVisible = false;

  auto restart = scene_manager->getEntity("restart");
  registry.get<engine::CShaderProgram>(restart).isVisible = false;

  auto menu = scene_manager->getEntity("menu");
  registry.get<engine::CShaderProgram>(menu).isVisible = false;

  app.togglePauseGame(false);
}

void PauseState::onUpdate(const engine::Time& ts) {
  engine::Application::Get().getSceneManager()->onUpdate(ts);
}

void PauseState::onEvent(engine::events::Event& e) {
  auto& registry = engine::Application::Get().getSceneManager()->getRegistry();

  dispatchers::appDispatcher(registry, e);
  dispatchers::inputDispatcher(registry, e);
}

std::unique_ptr<engine::State> PauseState::Create() {
  return std::make_unique<PauseState>();
}
}
#include "layers/game/gameoverOverlay.h"

#include "states/menuState.h"

namespace demos::layers {

GameoverOverlay::GameoverOverlay() : Layer("GameoverOverlay") {}

void GameoverOverlay::onAttach() {
  auto& app = engine::Application::Get();
  const auto& scene_manager = app.getSceneManager();
  auto& registry = app.getSceneManager()->getRegistry();

  app.togglePauseGame(true);

  auto bird = scene_manager->getEntity("bird");
  registry.get<engine::CShaderProgram>(bird).isVisible = false;

  auto gameover = scene_manager->getEntity("gameover");
  registry.get<engine::CShaderProgram>(gameover).isVisible = true;

  auto restart = scene_manager->getEntity("restart");
  registry.get<engine::CShaderProgram>(restart).isVisible = true;

  auto menu = scene_manager->getEntity("menu");
  registry.get<engine::CShaderProgram>(menu).isVisible = true;
}

void GameoverOverlay::onDetach() {
  auto& app = engine::Application::Get();
  const auto& scene_manager = app.getSceneManager();
  auto& registry = app.getSceneManager()->getRegistry();

  auto gameover = scene_manager->getEntity("gameover");
  registry.get<engine::CShaderProgram>(gameover).isVisible = false;

  auto restart = scene_manager->getEntity("restart");
  registry.get<engine::CShaderProgram>(restart).isVisible = false;

  auto menu = scene_manager->getEntity("menu");
  registry.get<engine::CShaderProgram>(menu).isVisible = false;

  app.togglePauseGame(false);
}

void GameoverOverlay::onEvent(engine::events::Event& e) {
  engine::events::EventDispatcher dispatcher(e);
  dispatcher.dispatch<engine::events::KeyPressedEvent>(
    BIND_EVENT(onKeyPressed));
}

bool GameoverOverlay::onKeyPressed(engine::events::KeyPressedEvent& e) {
  // repeat event if key is held down
  if (e.repeating())
    return false;

  auto& app = engine::Application::Get();

  if (e.getKeyCode() == engine::Key::R) {
    m_enabled = false;
    onDetach();
    app.getSettingsManager()->reloadScene = true;
    return true;
  } else if (e.getKeyCode() == engine::Key::M) {
    app.getStatesManager()->popState("GameState");
    // TODO delete this and import
    if (app.getStatesManager()->getStateIdx() == 0) {
      app.getSceneManager()->clearScene();
      app.getAssetsManager()->clear();
      app.getStatesManager()->pushState(states::MenuState::Create());
    }
    return true;
  } else if (e.getKeyCode() == engine::Key::P) {
    return true;
  }

  return false;
}

std::unique_ptr<engine::Layer> GameoverOverlay::Create() {
  return std::make_unique<GameoverOverlay>();
}
}
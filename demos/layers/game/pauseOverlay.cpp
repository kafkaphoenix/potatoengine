#include "layers/game/pauseOverlay.h"

#include "states/menuState.h"

namespace demos::layers {

PauseOverlay::PauseOverlay() : Layer("PauseOverlay") {}

void PauseOverlay::onAttach() {
  auto& app = engine::Application::Get();
  const auto& scene_manager = app.getSceneManager();
  auto& registry = app.getSceneManager()->getRegistry();

  app.togglePauseGame(true);

  auto bird = scene_manager->getEntity("bird");
  registry.get<engine::CShaderProgram>(bird).isVisible = false;

  // auto overlay = scene_manager->getEntity("overlay"); // TODO transparency
  // not working registry.get<engine::CShaderProgram>(overlay).isVisible = true;

  auto paused = scene_manager->getEntity("paused");
  registry.get<engine::CShaderProgram>(paused).isVisible = true;

  auto resume = scene_manager->getEntity("resume");
  registry.get<engine::CShaderProgram>(resume).isVisible = true;

  auto restart = scene_manager->getEntity("restart");
  registry.get<engine::CShaderProgram>(restart).isVisible = true;

  auto menu = scene_manager->getEntity("menu");
  registry.get<engine::CShaderProgram>(menu).isVisible = true;
}

void PauseOverlay::onDetach() {
  auto& app = engine::Application::Get();
  const auto& scene_manager = app.getSceneManager();
  auto& registry = app.getSceneManager()->getRegistry();

  // For when we unpause the game
  auto bird = scene_manager->getEntity("bird");
  registry.get<engine::CShaderProgram>(bird).isVisible = true;

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

void PauseOverlay::onEvent(engine::events::Event& e) {
  engine::events::EventDispatcher dispatcher(e);
  dispatcher.dispatch<engine::events::KeyPressedEvent>(
    BIND_EVENT(onKeyPressed));
}

bool PauseOverlay::onKeyPressed(engine::events::KeyPressedEvent& e) {
  // repeat event if key is held down
  if (e.repeating())
    return false;

  auto& app = engine::Application::Get();

  if (e.getKeyCode() == engine::Key::P) {
    m_enabled = false;
    onDetach();
    return true;
  } else if (e.getKeyCode() == engine::Key::R) {
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
  }

  return false;
}

std::unique_ptr<engine::Layer> PauseOverlay::Create() {
  return std::make_unique<PauseOverlay>();
}
}
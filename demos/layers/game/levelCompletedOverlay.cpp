#include "layers/game/levelCompletedOverlay.h"

#include "states/menuState.h"

namespace demos::layers {

LevelCompletedOverlay::LevelCompletedOverlay()
  : Layer("LevelCompletedOverlay") {}

void LevelCompletedOverlay::onAttach() {
  auto& app = engine::Application::Get();
  const auto& scene_manager = app.getSceneManager();
  auto& registry = app.getSceneManager()->getRegistry();

  app.togglePauseGame(true);

  auto bird = scene_manager->getEntity("bird");
  registry.get<engine::CShaderProgram>(bird).isVisible = false;

  auto completed = scene_manager->getEntity("completed");
  engine::CShape& cShape = registry.get<engine::CShape>(completed);
  cShape.size.y = 0.4;
  cShape.meshes.clear();
  cShape.createMesh();
  registry.get<engine::CTransform>(completed).position.y = 0.3;
  registry.get<engine::CShaderProgram>(completed).isVisible = true;

  auto restart = scene_manager->getEntity("restart");
  registry.get<engine::CShaderProgram>(restart).isVisible = true;

  auto menu = scene_manager->getEntity("menu");
  registry.get<engine::CShaderProgram>(menu).isVisible = true;
}

void LevelCompletedOverlay::onDetach() {
  auto& app = engine::Application::Get();
  const auto& scene_manager = app.getSceneManager();
  auto& registry = app.getSceneManager()->getRegistry();

  auto completed = scene_manager->getEntity("completed");
  registry.get<engine::CShaderProgram>(completed).isVisible = false;

  auto restart = scene_manager->getEntity("restart");
  registry.get<engine::CShaderProgram>(restart).isVisible = false;

  auto menu = scene_manager->getEntity("menu");
  registry.get<engine::CShaderProgram>(menu).isVisible = false;

  app.togglePauseGame(false);
}

void LevelCompletedOverlay::onEvent(engine::events::Event& e) {
  engine::events::EventDispatcher dispatcher(e);
  dispatcher.dispatch<engine::events::KeyPressedEvent>(
    BIND_EVENT(onKeyPressed));
}

bool LevelCompletedOverlay::onKeyPressed(engine::events::KeyPressedEvent& e) {
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

std::unique_ptr<engine::Layer> LevelCompletedOverlay::Create() {
  return std::make_unique<LevelCompletedOverlay>();
}
}
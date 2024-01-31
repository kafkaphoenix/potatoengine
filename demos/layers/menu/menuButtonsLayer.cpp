#include "layers/menu/menuButtonsLayer.h"

#include "dispatchers/menu/menuInputDispatcher.h"

namespace demos::layers {

MenuButtonsLayer::MenuButtonsLayer() : Layer("MenuButtonsLayer") {}

void MenuButtonsLayer::onAttach() {
  auto& app = engine::Application::Get();
  const auto& scene_manager = app.getSceneManager();
  auto& registry = app.getSceneManager()->getRegistry();

  auto start = scene_manager->getEntity("start");
  registry.get<engine::CShaderProgram>(start).isVisible = true;

  auto exit = scene_manager->getEntity("exit");
  registry.get<engine::CShaderProgram>(exit).isVisible = true;
}

void MenuButtonsLayer::onDetach() {
  auto& app = engine::Application::Get();
  const auto& scene_manager = app.getSceneManager();
  auto& registry = app.getSceneManager()->getRegistry();

  auto start = scene_manager->getEntity("start");
  registry.get<engine::CShaderProgram>(start).isVisible = false;

  auto exit = scene_manager->getEntity("exit");
  registry.get<engine::CShaderProgram>(exit).isVisible = false;
}

void MenuButtonsLayer::onEvent(engine::events::Event& e) {
  dispatchers::menuInputDispatcher(e);
}

std::unique_ptr<engine::Layer> MenuButtonsLayer::Create() {
  return std::make_unique<MenuButtonsLayer>();
}
}
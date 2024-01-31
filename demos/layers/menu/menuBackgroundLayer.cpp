#include "layers/menu/MenuBackgroundLayer.h"

namespace demos::layers {

MenuBackgroundLayer::MenuBackgroundLayer() : Layer("MenuBackgroundLayer") {}

void MenuBackgroundLayer::onAttach() {
  auto& app = engine::Application::Get();
  const auto& settings_manager = app.getSettingsManager();
  const auto& scene_manager = app.getSceneManager();
  auto& registry = scene_manager->getRegistry();

  entt::entity bird = scene_manager->getEntity("bird");
  registry.get<engine::CShaderProgram>(bird).isVisible = true;
  registry.get<engine::CTextureAtlas>(bird).index = 0;
  registry.get<engine::CTransform>(bird).position.y = 0.75;

  entt::entity title = scene_manager->getEntity("title");
  registry.get<engine::CShaderProgram>(title).isVisible = true;
}

void MenuBackgroundLayer::onDetach() {
  auto& app = engine::Application::Get();
  const auto& scene_manager = app.getSceneManager();
  auto& registry = app.getSceneManager()->getRegistry();

  auto bird = scene_manager->getEntity("bird");
  registry.get<engine::CShaderProgram>(bird).isVisible = false;

  auto title = scene_manager->getEntity("title");
  registry.get<engine::CShaderProgram>(title).isVisible = false;
}

std::unique_ptr<engine::Layer> MenuBackgroundLayer::Create() {
  return std::make_unique<MenuBackgroundLayer>();
}
}
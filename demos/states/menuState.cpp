#include "states/menuState.h"

#include "dispatchers/common/inputDispatcher.h"
#include "dispatchers/common/windowDispatcher.h"
#include "layers/menu/menuBackgroundLayer.h"
#include "layers/menu/menuButtonsLayer.h"
#include "systems/graphics/sRender.h"

namespace demos::states {

MenuState::MenuState() : State("MenuState") {
  auto& app = engine::Application::Get();
  const auto& settings_manager = app.getSettingsManager();
  const auto& scene_manager = app.getSceneManager();

  // TODO move to loading screen state
  scene_manager->createScene(settings_manager->activeScene,
                             settings_manager->activeScenePath);
}

void MenuState::onAttach() {
  auto& app = engine::Application::Get();
  const auto& scene_manager = app.getSceneManager();
  const auto& states_manager = engine::Application::Get().getStatesManager();
  states_manager->pushLayer(layers::MenuBackgroundLayer::Create());
  m_layersManager->pushLayer(layers::MenuButtonsLayer::Create());
  engine::Application::Get().getSceneManager()->registerSystem(
    "render_system", std::make_unique<systems::RenderSystem>(100));
}

void MenuState::onDetach() {
  auto& app = engine::Application::Get();
  const auto& scene_manager = app.getSceneManager();
  const auto& states_manager = engine::Application::Get().getStatesManager();
  auto& registry = app.getSceneManager()->getRegistry();

  registry
    .view<engine::CShaderProgram, engine::CTransform, engine::CName,
          engine::CUUID>()
    .each([&](engine::CShaderProgram& cShaderProgram,
              engine::CTransform& cTransform, const engine::CName& cName,
              const engine::CUUID& cUUID) {
      if (cName.name == "title") {
        cShaderProgram.isVisible = false;
      } else if (cName.name == "start") {
        cShaderProgram.isVisible = false;
      } else if (cName.name == "exit") {
        cShaderProgram.isVisible = false;
      }
    });

  states_manager->getCurrentState()->getLayersManager()->clear();
  scene_manager->unregisterSystem("render_system");
}

void MenuState::onEvent(engine::events::Event& e) {
  auto& registry = engine::Application::Get().getSceneManager()->getRegistry();
  dispatchers::windowDispatcher(e, registry);
  dispatchers::inputDispatcher(e, registry);

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

std::unique_ptr<engine::State> MenuState::Create() {
  return std::make_unique<MenuState>();
}
}
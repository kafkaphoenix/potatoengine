#include "states/menuState.h"

#include "components/core/cState.h"
#include "dispatchers/appDispatcher.h"
#include "dispatchers/inputDispatcher.h"
#include "systems/animation/sAnimation.h"
#include "systems/graphics/sRender.h"

namespace demos::states {

MenuState::MenuState() : State("MenuState") {}

void MenuState::onAttach() {
  auto& app = engine::Application::Get();
  const auto& settings = app.getSettings();
  const auto& scene_manager = app.getSceneManager();

  scene_manager->createScene(settings->activeScene, settings->activeScenePath);
  entt::entity gamestate = scene_manager->getEntity("gamestate");
  CState& state = scene_manager->getRegistry().get<CState>(gamestate);
  state.state = CState::State::MENU;
  entt::entity bird = scene_manager->getEntity("bird");
  scene_manager->getRegistry().get<engine::CTransform>(bird).position.y = 0.75;
  scene_manager->registerSystem("render_system",
                                std::make_unique<systems::RenderSystem>(100));
}

void MenuState::onDetach() {
  auto& app = engine::Application::Get();
  const auto& scene_manager = app.getSceneManager();
  auto& registry = app.getSceneManager()->getRegistry();

  auto title = scene_manager->getEntity("title");
  registry.get<engine::CShaderProgram>(title).isVisible = false;

  auto start = scene_manager->getEntity("start");
  registry.get<engine::CShaderProgram>(start).isVisible = false;

  auto exit = scene_manager->getEntity("exit");
  registry.get<engine::CShaderProgram>(exit).isVisible = false;

  auto bird = scene_manager->getEntity("bird");
  registry.get<engine::CShaderProgram>(bird).isVisible = false;

  scene_manager->unregisterSystem("render_system");
}

void MenuState::onUpdate(const engine::Time& ts) {
  engine::Application::Get().getSceneManager()->onUpdate(ts);
}

void MenuState::onEvent(engine::events::Event& e) {
  auto& registry = engine::Application::Get().getSceneManager()->getRegistry();

  dispatchers::appDispatcher(registry, e);
  dispatchers::inputDispatcher(registry, e);
}

std::unique_ptr<engine::State> MenuState::Create() {
  return std::make_unique<MenuState>();
}
}
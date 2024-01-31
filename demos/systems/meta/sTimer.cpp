#include "systems/meta/sTimer.h"

#include "components/meta/cTimer.h"

namespace demos::systems {

TimerSystem::~TimerSystem() {
  auto& registry = engine::Application::Get().getSceneManager()->getRegistry();
  for (auto e : registry.view<engine::CTag, engine::CUUID>()) {
    if (registry.get<engine::CTag>(e).tag == "timer") {
      registry.destroy(e);
    }
  }
}

void TimerSystem::init(entt::registry& registry) {
  auto& app = engine::Application::Get();
  const auto& scene_manager = app.getSceneManager();

  entt::entity unidades =
    scene_manager->createEntity("scene", "numbers", "timer_unidades", "timer");
  entt::entity decenas =
    scene_manager->createEntity("scene", "numbers", "timer_decenas", "timer");
  entt::entity centenas =
    scene_manager->createEntity("scene", "numbers", "timer_centenas", "timer");

  registry.get<engine::CTransform>(unidades).position = {-1.3f, 0.8f, 0.f};
  registry.get<engine::CTransform>(decenas).position = {-1.45f, 0.8f, 0.f};
  registry.get<engine::CTransform>(centenas).position = {-1.6f, 0.8f, 0.f};

  entt::entity gamestate = registry.view<CTimer, engine::CUUID>().front();
  uint32_t left = registry.get<CTimer>(gamestate).left;

  registry.get<engine::CTextureAtlas>(centenas).index = left / 100;
  registry.get<engine::CTextureAtlas>(decenas).index = (left % 100) / 10;
  registry.get<engine::CTextureAtlas>(unidades).index = left % 10;

  registry.get<engine::CShaderProgram>(unidades).isVisible = true;
  registry.get<engine::CShaderProgram>(decenas).isVisible = true;
  registry.get<engine::CShaderProgram>(centenas).isVisible = true;

  app.getRenderManager()->reorder();
}

}
#include "systems/world/sLight.h"

using namespace entt::literals;

namespace demos::systems {

void LightSystem::update(entt::registry& registry, const engine::Time& ts) {
  if (engine::Application::Get().isGamePaused()) {
    return;
  }

  registry.view<engine::CLight, engine::CTransform, engine::CUUID>().each(
    [&](const engine::CLight& cLight, const engine::CTransform& cTransform,
        const engine::CUUID& cUUID) {
      entt::monostate<"lightPosition"_hs>{} = cTransform.position;
      entt::monostate<"lightColor"_hs>{} = cLight.color;
    });
}
}
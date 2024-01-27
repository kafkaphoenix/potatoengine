#include "systems/core/sDelete.h"

namespace demos::systems {

void DeleteSystem::update(entt::registry& registry, const engine::Time& ts) {
  if (engine::Application::Get().isGamePaused()) {
    return;
  }

  auto to_destroy = registry.view<engine::CDeleted, engine::CUUID>();
  registry.destroy(to_destroy.begin(), to_destroy.end());
}
}
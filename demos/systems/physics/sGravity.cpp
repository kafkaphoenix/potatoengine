#include "systems/physics/sGravity.h"

namespace demos::systems {

void GravitySystem::update(entt::registry& registry, const engine::Time& ts) {
  if (engine::Application::Get().isGamePaused()) {
    return;
  }

  registry
    .view<engine::CTransform, engine::CRigidBody, engine::CGravity,
          engine::CUUID>()
    .each([&](engine::CTransform& cTransform,
              const engine::CRigidBody& cRigidBody,
              const engine::CGravity& cGravity, const engine::CUUID& cUUID) {
      if (cRigidBody.isKinematic) {
        cTransform.position.y -= cGravity.acceleration * ts;
      }
    });
}
}
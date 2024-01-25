#include "systems/physics/sCollision.h"

namespace demos::systems {

void CollisionSystem::update(entt::registry& registry, const engine::Time& ts) {
  auto& app = engine::Application::Get();
  if (app.isGamePaused()) {
    return;
  }
  const auto& settings = app.getSettings();

  registry
    .view<engine::CTransform, engine::CRigidBody, engine::CCollider,
          engine::CUUID>()
    .each([&](entt::entity e, engine::CTransform& cTransform,
              const engine::CRigidBody& cRigidBody,
              const engine::CCollider& cCollider, const engine::CUUID& cUUID) {
      if (cRigidBody.isKinematic) {
        bool collided = false;

        registry
          .view<engine::CTransform, engine::CRigidBody, engine::CCollider,
                engine::CUUID>()
          .each([&](entt::entity e2, const engine::CTransform& cTransform2,
                    const engine::CRigidBody& cRigidBody2,
                    const engine::CCollider& cCollider2,
                    const engine::CUUID& cUUID2) {
            if (cUUID.uuid != cUUID2.uuid) {
              if (cCollider.type == engine::CCollider::Type::Rectangle and
                  cCollider2.type == engine::CCollider::Type::Rectangle) {
                float minX = cTransform.position.x - cCollider.size.x / 2;
                float maxX = cTransform.position.x + cCollider.size.x / 2;
                float minY = cTransform.position.y - cCollider.size.y / 2;
                float maxY = cTransform.position.y + cCollider.size.y / 2;

                float minX2 = cTransform2.position.x - cCollider2.size.x / 2;
                float maxX2 = cTransform2.position.x + cCollider2.size.x / 2;
                float minY2 = cTransform2.position.y - cCollider2.size.y / 2;
                float maxY2 = cTransform2.position.y + cCollider2.size.y / 2;

                if (minX < maxX2 and maxX > minX2 and minY < maxY2 and
                    maxY > minY2) {
                  collided = true;
                }
              }
            }
          });

        if (collided) {
          settings->reloadScene = true;
        }

        // top off screen check
        if (cTransform.position.y > 0.9) {
          cTransform.position.y = 0.9;
        }
      }
    });
}
}

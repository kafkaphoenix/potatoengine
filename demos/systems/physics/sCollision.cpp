#include "systems/physics/sCollision.h"

namespace demos::systems {

void CollisionSystem::update(entt::registry& registry, const engine::Time& ts) {
  auto& app = engine::Application::Get();
  if (app.isGamePaused()) {
    return;
  }
  const auto& settings = app.getSettings();

  entt::entity bird = app.getSceneManager()->getEntity("bird");
  engine::CTransform& cTransform = registry.get<engine::CTransform>(bird);
  const engine::CCollider& cCollider = registry.get<engine::CCollider>(bird);
  const engine::CUUID& cUUID = registry.get<engine::CUUID>(bird);
  std::string_view collidedWith = "";

  for (auto e : registry.view<engine::CTransform, engine::CCollider,
                              engine::CTag, engine::CUUID>()) {
    const engine::CTransform& cTransform2 = registry.get<engine::CTransform>(e);
    const engine::CCollider& cCollider2 = registry.get<engine::CCollider>(e);
    const engine::CTag& cTag2 = registry.get<engine::CTag>(e);
    const engine::CUUID& cUUID2 = registry.get<engine::CUUID>(e);
    if (cUUID.uuid != cUUID2.uuid) {
      if (cCollider.type == engine::CCollider::Type::Rectangle and
          cCollider2.type == engine::CCollider::Type::Rectangle) {
        // TODO move logic to class physics
        float minX = cTransform.position.x - cCollider.size.x / 2;
        float maxX = cTransform.position.x + cCollider.size.x / 2;
        float minY = cTransform.position.y - cCollider.size.y / 2;
        float maxY = cTransform.position.y + cCollider.size.y / 2;

        float minX2 = cTransform2.position.x - cCollider2.size.x / 2;
        float maxX2 = cTransform2.position.x + cCollider2.size.x / 2;
        float minY2 = cTransform2.position.y - cCollider2.size.y / 2;
        float maxY2 = cTransform2.position.y + cCollider2.size.y / 2;

        if (minX < maxX2 and maxX > minX2 and minY < maxY2 and maxY > minY2) {
          collidedWith = cTag2.tag;
          if (collidedWith == "coin") {
            registry.get<engine::CShaderProgram>(e).isVisible = false;
          }
          break;
        }
      }
    }
  }

  if (collidedWith == "pipe" or collidedWith == "ground") {
    settings->reloadScene = true;
  } else if (collidedWith == "coin") {
    engine::events::AppUpdateEvent event("onCoinCollected");
    app.getWindow()->triggerEvent(event);
  }

  // top off screen check
  if (cTransform.position.y > 0.9) {
    cTransform.position.y = 0.9;
  }
}

}

#pragma once

#include <entt/entt.hpp>
#include <glm/glm.hpp>

#include "core/time.h"
#include "scene/components/common/cUUID.h"
#include "scene/components/physics/cCollider.h"
#include "scene/components/physics/cGravity.h"
#include "scene/components/physics/cRigidBody.h"
#include "scene/components/physics/cTransform.h"

namespace potatoengine {

void collisionSystem(entt::registry& r, Time ts) {
  r.view<CTransform, CRigidBody, CCollider, CUUID>().each(
    [&](entt::entity e, CTransform& cTransform, const CRigidBody& cRigidBody,
        const CCollider& cCollider, const CUUID& cUUID) {
      if (cRigidBody.isKinematic) {
        bool collided = false;

        r.view<CTransform, CRigidBody, CCollider, CUUID>().each(
          [&](entt::entity e2, CTransform& cTransform2,
              const CRigidBody& cRigidBody2, const CCollider& cCollider2,
              const CUUID& cUUID2) {
            if (cUUID.uuid != cUUID2.uuid) {
              if (cCollider.type == CCollider::Type::Rectangle and
                  cCollider2.type == CCollider::Type::Rectangle) {
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
          CGravity* cGravity = r.try_get<CGravity>(e);
          float contraspeed = cGravity->acceleration * ts;
          cTransform.position.y -= contraspeed;

          if (cTransform.position.y < 0) {
            cTransform.position.y = 0.5;
          }
        }
      }
    });
}
}

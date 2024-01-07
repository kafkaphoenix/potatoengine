#pragma once

#include <entt/entt.hpp>
#include <glm/glm.hpp>

#include "core/time.h"
#include "scene/components/common/cUUID.h"
#include "scene/components/physics/cRigidBody.h"
#include "scene/components/physics/cTransform.h"
#include "scene/components/physics/cGravity.h"

namespace potatoengine {

void gravitySystem(entt::registry& r, Time ts) {
  r.view<CTransform, CRigidBody, CGravity, CUUID>().each(
    [&](entt::entity e, CTransform& cTransform, const CRigidBody& cRigidBody, const CGravity& cGravity,
        const CUUID& cUUID) {

      if (cRigidBody.isKinematic) {
        float speed = cGravity.acceleration * ts;

        cTransform.position.y -= speed;
      }
    });
}
}

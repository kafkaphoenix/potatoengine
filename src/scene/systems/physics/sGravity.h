#pragma once

#include <entt/entt.hpp>
#include <glm/glm.hpp>

#include "core/application.h"
#include "core/time.h"
#include "scene/components/common/cUUID.h"
#include "scene/components/physics/cGravity.h"
#include "scene/components/physics/cRigidBody.h"
#include "scene/components/physics/cTransform.h"

namespace potatoengine {

void gravitySystem(entt::registry& registry, const Time& ts) {
  registry.view<CTransform, CRigidBody, CGravity, CUUID>().each(
    [&](entt::entity e, CTransform& cTransform, const CRigidBody& cRigidBody,
        const CGravity& cGravity, const CUUID& cUUID) {
      if (cRigidBody.isKinematic) {
        cTransform.position.y -= cGravity.acceleration * ts;
      }
    });
}
}

#pragma once

#include <entt/entt.hpp>
#include <glm/gtx/string_cast.hpp>

#include "core/application.h"
#include "scene/components/common/cUUID.h"
#include "scene/components/physics/cTransform.h"

namespace potatoengine {

void printSystem(entt::registry& registry) {
  ENGINE_DEBUG("================================");
  registry.view<CTransform, CUUID>().each(
    [&](entt::entity e, const CTransform& cTransform, const CUUID& cUUID) {
      ENGINE_DEBUG(
        "[{0}] position({1}) rotation({2})", entt::to_integral(e),
        glm::to_string(cTransform.position),
        glm::to_string(glm::degrees(glm::eulerAngles(cTransform.rotation))));
    });
}
}
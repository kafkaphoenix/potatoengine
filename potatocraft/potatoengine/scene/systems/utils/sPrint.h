#pragma once

#include <entt/entt.hpp>

#include "potatoengine/scene/components/common/cUUID.h"
#include "potatoengine/scene/components/physics/cTransform.h"

namespace potatoengine {

void printSystem(entt::registry& r) {
    CORE_INFO("================================");
    r.view<CTransform, CUUID>().each([&](entt::entity e, const CTransform& cTransform, const CUUID& cUUID) {
        CORE_INFO("position ({0}): {1}, {2}, {3}", entt::to_integral(e), cTransform.position.x, cTransform.position.y, cTransform.position.z);
    });
}
}
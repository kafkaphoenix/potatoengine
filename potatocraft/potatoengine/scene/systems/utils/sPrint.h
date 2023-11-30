#pragma once

#include <entt/entt.hpp>

#include "potatoengine/scene/components/common/cUUID.h"
#include "potatoengine/scene/components/physics/cTransform.h"

namespace potatoengine {

void printSystem(entt::registry& r) {
    ENGINE_TRACE("================================");
    r.view<CTransform, CUUID>().each([&](entt::entity e, const CTransform& cTransform, const CUUID& cUUID) {
        ENGINE_TRACE("[{0}] position({1}) rotation({2})", entt::to_integral(e), cTransform.position, glm::degrees(glm::eulerAngles(cTransform.rotation)));
    });
}
}
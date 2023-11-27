#pragma once

#include <entt/entt.hpp>
#include <glm/gtx/string_cast.hpp>

#include "potatoengine/scene/components/common/cUUID.h"
#include "potatoengine/scene/components/physics/cTransform.h"

namespace potatoengine {

void printSystem(entt::registry& r) {
    CORE_TRACE("================================");
    r.view<CTransform, CUUID>().each([&](entt::entity e, const CTransform& cTransform, const CUUID& cUUID) {
        CORE_TRACE("[{0}] position({1}) rotation({2})", entt::to_integral(e), glm::to_string(cTransform.position), glm::to_string(glm::degrees(glm::eulerAngles(cTransform.rotation))));
    });
}
}
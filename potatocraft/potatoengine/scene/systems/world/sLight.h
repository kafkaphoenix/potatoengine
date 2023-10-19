#pragma once

#include <entt/entt.hpp>

#include "potatoengine/scene/components/common/cUUID.h"
#include "potatoengine/scene/components/world/cLight.h"
#include "potatoengine/scene/components/physics/cTransform.h"

namespace potatoengine {

void lightSystem(entt::registry& reg) {
    using namespace entt::literals;
    reg.view<CLight, CTransform, CUUID>().each([&](const CLight& cLight, const CTransform& cTransform, const CUUID& cUUID) {
        entt::monostate<"lightPosition"_hs>{} = cTransform.position;
        entt::monostate<"lightColor"_hs>{} = cLight.color;
    });
}
}
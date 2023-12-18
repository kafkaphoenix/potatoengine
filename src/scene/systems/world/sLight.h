#pragma once

#include <entt/entt.hpp>

#include "scene/components/common/cUUID.h"
#include "scene/components/physics/cTransform.h"
#include "scene/components/world/cLight.h"

using namespace entt::literals;

namespace potatoengine {

void lightSystem(entt::registry& reg) {
  reg.view<CLight, CTransform, CUUID>().each([&](const CLight& cLight,
                                                 const CTransform& cTransform,
                                                 const CUUID& cUUID) {
    entt::monostate<"lightPosition"_hs>{} = cTransform.position;
    entt::monostate<"lightColor"_hs>{} = cLight.color;
  });
}
}
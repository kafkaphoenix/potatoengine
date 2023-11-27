#pragma once

#include <entt/entt.hpp>

#include "potatoengine/core/time.h"
#include "potatoengine/pch.h"
#include "potatoengine/scene/systems/utils/sDelete.h"
#include "potatoengine/scene/systems/utils/sPrint.h"
#include "potatoengine/scene/systems/physics/sMovement.h"
#include "potatoengine/scene/systems/graphics/sRender.h"
#include "potatoengine/scene/systems/world/sSkybox.h"
#include "potatoengine/scene/systems/world/sTime.h"
#include "potatoengine/scene/systems/world/sLight.h"
#include "potatoengine/scene/systems/world/sTerrain.h"

namespace potatoengine {

void updateSystem(entt::registry& reg, std::weak_ptr<Renderer> r, const Time& ts) {
    deleteSystem(reg);
    timeSystem(reg, ts);
    terrainSystem(reg);
    lightSystem(reg);
    movementSystem(reg, ts);
    skyboxSystem(reg);
    renderSystem(reg, r);
    // printSystem(reg); only for debugging purposes
}
}
#pragma once

#include <entt/entt.hpp>

#include "core/time.h"
#include "pch.h"
#include "scene/systems/graphics/sRender.h"
#include "scene/systems/physics/sMovement.h"
#include "scene/systems/utils/sDelete.h"
#include "scene/systems/utils/sPrint.h"
#include "scene/systems/world/sLight.h"
#include "scene/systems/world/sSkybox.h"
#include "scene/systems/world/sTerrain.h"
#include "scene/systems/world/sTime.h"
#include "scene/systems/physics/sGravity.h"
#include "scene/systems/physics/sCollision.h"

namespace potatoengine {

void updateSystem(entt::registry& reg, std::weak_ptr<Renderer> r, const Time& ts) {
  deleteSystem(reg);
  timeSystem(reg, ts);
  terrainSystem(reg);
  lightSystem(reg);
  collisionSystem(reg, ts);
  gravitySystem(reg, ts);
  movementSystem(reg, ts);
  skyboxSystem(reg);
  renderSystem(reg, r);
  // printSystem(reg); only for debugging purposes
}
}
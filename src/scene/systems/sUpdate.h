#pragma once

#include <entt/entt.hpp>

#include "core/application.h"
#include "core/time.h"
#include "scene/systems/animation/sAnimation.h"
#include "scene/systems/graphics/sRender.h"
#include "scene/systems/physics/sCollision.h"
#include "scene/systems/physics/sGravity.h"
#include "scene/systems/physics/sMovement.h"
#include "scene/systems/terrain/sPipes.h"
#include "scene/systems/terrain/sTerrain.h"
#include "scene/systems/utils/sDelete.h"
#include "scene/systems/world/sLight.h"
#include "scene/systems/world/sSkybox.h"
#include "scene/systems/world/sTime.h"

namespace potatoengine {

void updateSystem(entt::registry& registry, const Time& ts) {
  auto& app = Application::Get();
  bool isGamePaused = app.isGamePaused();
  if (not isGamePaused) {
    deleteSystem(registry);
    timeSystem(registry, ts);
    terrainSystem(registry);
    pipesSystem(registry);
    skyboxSystem(registry);
    lightSystem(registry);
    collisionSystem(registry, ts);
    gravitySystem(registry, ts);
    animationSystem(registry, ts);
    movementSystem(registry, ts);
  }
  renderSystem(registry);
}
}
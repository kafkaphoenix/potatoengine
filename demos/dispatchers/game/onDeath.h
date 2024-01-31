#pragma once

#include <entt/entt.hpp>

#include "engineAPI.h"
#include "layers/game/gameoverOverlay.h"

namespace demos::dispatchers {

inline void onDeath(entt::registry& registry) {
  engine::Application::Get().getStatesManager()->pushOverlay(layers::GameoverOverlay::Create(), true);
}
}
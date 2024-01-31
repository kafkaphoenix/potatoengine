#pragma once

#include <entt/entt.hpp>

#include "engineAPI.h"
#include "layers/game/levelCompletedOverlay.h"

namespace demos::dispatchers {

inline void onLevelCompleted(entt::registry& registry) {
  auto& app = engine::Application::Get();
  app.getStatesManager()->pushOverlay(layers::LevelCompletedOverlay::Create(), true);
}
}
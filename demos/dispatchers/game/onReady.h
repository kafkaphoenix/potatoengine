#pragma once

#include <entt/entt.hpp>

#include "engineAPI.h"

namespace demos::dispatchers {

inline void onReady(entt::registry& registry) {
  engine::Application::Get().getStatesManager()->disableOverlay("GetReadyOverlay");
}
}
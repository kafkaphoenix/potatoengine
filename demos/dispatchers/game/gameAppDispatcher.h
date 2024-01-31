#pragma once

#include <entt/entt.hpp>

#include "dispatchers/game/onCoinCollected.h"
#include "dispatchers/game/onLevelCompleted.h"
#include "dispatchers/game/onDeath.h"
#include "dispatchers/game/onReady.h"
#include "dispatchers/game/onTimerTicked.h"
#include "engineAPI.h"

namespace demos::dispatchers {

inline bool onGameTick(engine::events::AppTickEvent& e,
                       entt::registry& registry) {
  onTimerTicked(registry);
  return true;
}

inline bool onGameUpdate(engine::events::AppUpdateEvent& e,
                         entt::registry& registry) {
  if (e.getDispatcherTarget() == "onCoinCollected") {
    onCoinCollected(registry);
    return true;
  } else if (e.getDispatcherTarget() == "onDeath") {
    onDeath(registry);
    return true;
  } else if (e.getDispatcherTarget() == "onReady") {
    onReady(registry);
    return true;
  } else if (e.getDispatcherTarget() == "onLevelCompleted") {
    onLevelCompleted(registry);
    return true;
  }

  return false;
}

inline bool onGameRender(engine::events::AppRenderEvent& e) { return true; }

inline void gameAppDispatcher(engine::events::Event& e,
                              entt::registry& registry) {
  engine::events::EventDispatcher dispatcher(e);

  dispatcher.dispatch<engine::events::AppTickEvent>(
    BIND_STATIC_EVENT(onGameTick, registry));
  dispatcher.dispatch<engine::events::AppUpdateEvent>(
    BIND_STATIC_EVENT(onGameUpdate, registry));
  dispatcher.dispatch<engine::events::AppRenderEvent>(
    BIND_STATIC_EVENT(onGameRender));
}
}
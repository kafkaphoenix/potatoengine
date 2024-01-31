#pragma once

#include <entt/entt.hpp>

#include "components/meta/cTimer.h"
#include "engineAPI.h"

namespace demos::dispatchers {

inline void onTimerTicked(entt::registry& registry) {
  entt::entity gamestate = registry.view<CTimer, engine::CUUID>().front();
  CTimer& timer = registry.get<CTimer>(gamestate);
  int left = --timer.left;
  if (timer.maxTime - left == 3) {
    engine::events::AppUpdateEvent event("onReady");
    engine::Application::Get().getWindowsManager()->triggerEvent(event);
  }
  if (left < 0) {
    engine::events::AppUpdateEvent event("onLevelCompleted");
    engine::Application::Get().getWindowsManager()->triggerEvent(event);
  } else {
    registry.view<engine::CTextureAtlas, engine::CName, engine::CUUID>().each(
      [&](entt::entity e, engine::CTextureAtlas& cTextureAtlas,
          const engine::CName& cName, const engine::CUUID& cUUID) {
        if (cName.name == "timer_unidades") {
          cTextureAtlas.index = left % 10;
        } else if (cName.name == "timer_decenas") {
          cTextureAtlas.index = (left % 100) / 10;
        } else if (cName.name == "timer_centenas") {
          cTextureAtlas.index = left / 100;
        } else if (cName.name == "countdown") {
          cTextureAtlas.index -= 1;
        }
      });
  }
}
}
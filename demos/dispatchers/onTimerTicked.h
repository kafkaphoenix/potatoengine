#pragma once

#include <entt/entt.hpp>

#include "components/core/cState.h"
#include "components/meta/cTimer.h"
#include "engineAPI.h"

namespace demos::dispatchers {

inline bool onTimerTicked(entt::registry& registry) {
  entt::entity gamestate = registry.view<CTimer, engine::CUUID>().front();
  CTimer& timer = registry.get<CTimer>(gamestate);
  CState& state = registry.get<CState>(gamestate);
  int left = --timer.left;
  if (timer.maxTime - left == 5) {
    state.state = CState::State::READY;
  } else if (left == 0) {
    state.state = CState::State::STOPPED;
  }
  registry.view<engine::CTextureAtlas, engine::CName, engine::CUUID>().each(
    [&](entt::entity e, engine::CTextureAtlas& cTextureAtlas, const engine::CName& cName,
        const engine::CUUID& cUUID) {
      if (cName.name == "timer_unidades") {
        cTextureAtlas.index = left % 10;
      } else if (cName.name == "timer_decenas") {
        cTextureAtlas.index = (left % 100) / 10;
      } else if (cName.name == "timer_centenas") {
        cTextureAtlas.index = left / 100;
      } else if (cName.name == "countdown") {
        cTextureAtlas.index -= 1;
        if (cTextureAtlas.index == 0) {
          registry.emplace<engine::CDeleted>(e);
        }
      }
    });

  return true;
}
}
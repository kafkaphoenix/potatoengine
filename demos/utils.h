#pragma once

#include <entt/entt.hpp>

#include "components/config/cCoins.h"
#include "components/config/cPipes.h"
#include "components/core/cState.h"
#include "components/meta/cScore.h"
#include "components/meta/cTimer.h"
#include "engineAPI.h"

using namespace entt::literals;

namespace demos {

inline CCoins& CastCCoins(void* other) { return *static_cast<CCoins*>(other); }

inline CPipes& CastCPipes(void* other) { return *static_cast<CPipes*>(other); }

inline CScore& CastCScore(void* other) { return *static_cast<CScore*>(other); }

inline CTimer& CastCTimer(void* other) { return *static_cast<CTimer*>(other); }

inline CState& CastCState(void* other) { return *static_cast<CState*>(other); }

inline void RegisterComponents() {
  entt::meta<CCoins>()
    .type("coins"_hs)
    .ctor<&CastCCoins, entt::as_ref_t>()
    .data<&CCoins::maxCoins>("maxCoins"_hs)
    .data<&CCoins::coins>("coins"_hs)
    .func<&CCoins::print>("print"_hs)
    .func<&CCoins::getInfo>("getInfo"_hs)
    .func<&engine::assign<CCoins>, entt::as_ref_t>("assign"_hs);

  entt::meta<CPipes>()
    .type("pipes"_hs)
    .ctor<&CastCPipes, entt::as_ref_t>()
    .data<&CPipes::maxPipes>("maxPipes"_hs)
    .data<&CPipes::pipes>("pipes"_hs)
    .func<&CPipes::print>("print"_hs)
    .func<&CPipes::getInfo>("getInfo"_hs)
    .func<&engine::assign<CPipes>, entt::as_ref_t>("assign"_hs);

  entt::meta<CScore>()
    .type("score"_hs)
    .ctor<&CastCScore, entt::as_ref_t>()
    .data<&CScore::score>("score"_hs)
    .func<&CScore::print>("print"_hs)
    .func<&CScore::getInfo>("getInfo"_hs)
    .func<&engine::assign<CScore>, entt::as_ref_t>("assign"_hs);

  entt::meta<CTimer>()
    .type("timer"_hs)
    .ctor<&CastCTimer, entt::as_ref_t>()
    .data<&CTimer::maxTime>("maxTime"_hs)
    .data<&CTimer::left>("left"_hs)
    .func<&CTimer::print>("print"_hs)
    .func<&CTimer::getInfo>("getInfo"_hs)
    .func<&engine::assign<CTimer>, entt::as_ref_t>("assign"_hs);

  entt::meta<CState>()
    .type("state"_hs)
    .ctor<&CastCState, entt::as_ref_t>()
    .data<&CState::state>("state"_hs)
    .func<&CState::print>("print"_hs)
    .func<&CState::getInfo>("getInfo"_hs)
    .func<&engine::assign<CState>, entt::as_ref_t>("assign"_hs);
}
}
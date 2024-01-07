#pragma once

#include <entt/entt.hpp>

#include "core/application.h"
#include "core/time.h"
#include "scene/components/world/cTime.h"

namespace potatoengine {

void timeSystem(entt::registry& registry, const Time& ts) {
  registry.view<CTime>().each([&](CTime& cTime) {
    // We call it fps frames per second instead of 1 but the other functions
    // divide by fps so it is ok
    float timeScale = 86400.f / (cTime.dayLength * 3600.f);
    cTime.seconds += ts.getSeconds() * timeScale * cTime.acceleration;
    if (cTime.seconds >= 86400) {
      cTime.seconds = 0;
    }
    // dividing by fps because we call this function fps frames per second
    // instead of 1 but at the same time it helps us to see the time correctly
    // per frame so the logic works for 24 hours
    // TODO revisit probably will break with different fps
    cTime.currentHour = static_cast<int>(cTime.seconds / cTime.fps) / cTime.fps;
    cTime.currentMinute =
      static_cast<int>(cTime.seconds / cTime.fps) % cTime.fps;
    cTime.currentSecond = static_cast<int>(cTime.seconds) % cTime.fps;
  });
}
}
#include "systems/core/sTime.h"

namespace demos::systems {

void TimeSystem::update(entt::registry& registry, const engine::Time& ts) {
  auto& app = engine::Application::Get();
  if (app.isGamePaused()) {
    return;
  }
  const auto& settings = app.getSettings(); // TODO remove this for demos

  registry.view<engine::CTime, engine::CUUID>().each([&](engine::CTime& cTime, const engine::CUUID& cUUID) {
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
    int lastSecond = cTime.currentSecond;
    cTime.currentSecond = static_cast<int>(cTime.seconds) % cTime.fps;

    // tick every real second
    if (lastSecond != cTime.currentSecond) {
      // then we update minutes and hours
      if (cTime.currentSecond == 0) {
        cTime.currentMinute++;
        if (settings->activeScene == "Flappy Bird") {
          engine::events::AppTickEvent event;
          app.getWindow()->triggerEvent(event);
        }
        if (cTime.currentMinute == 60) {
          cTime.currentMinute = 0;
          cTime.currentHour++;
          if (cTime.currentHour == 24) {
            cTime.currentHour = 0;
          }
        }
      }
    }
  });
}
}
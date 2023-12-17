#pragma once

#include "scene/entity.h"
namespace potatoengine {

struct CTime {
    float seconds{};
    int currentHour{};
    int currentMinute{};
    int currentSecond{};
    float dayLength{0.5};
    float startingTime{7};
    float dayTransitionStart{5};
    float dayStart{7};
    float nightTransitionStart{19};
    float nightStart{21};
    float acceleration{1.f};
    int fps{60};

    CTime() = default;
    explicit CTime(float s, int ch, int cm, int cs, float st, float dl, float ns, float dts, float ds, float nts,
                   float a, int f)
        : seconds(s), currentHour(ch), currentMinute(cm), currentSecond(cs), startingTime(st), dayLength(dl),
          nightStart(ns), dayTransitionStart(dts), dayStart(ds), nightTransitionStart(nts), acceleration(a), fps(f) {}

    void print() const {
      ENGINE_BACKTRACE("\t\tseconds: {0}\n\t\t\t\tcurrentHour: {1}\n\t\t\t\tcurrentMinute: {2}\n\t\t\t\tcurrentSecond: "
                   "{3}\n\t\t\t\tdayLength: {4}\n\t\t\t\tstartingTime: {5}\n\t\t\t\tdayTransitionStart: "
                   "{6}\n\t\t\t\tdayStart: {7}\n\t\t\t\tnightTransitionStart: {8}\n\t\t\t\tnightStart: "
                   "{9}\n\t\t\t\tacceleration: {10}\n\t\t\t\tfps: {11}",
                   seconds, currentHour, currentMinute, currentSecond, dayLength, startingTime, dayTransitionStart,
                   dayStart, nightTransitionStart, nightStart, acceleration, fps);
    }

    void setTime(float time) {
      seconds = time * 3600.f;
      currentHour = static_cast<int>(seconds / 3600.f);
      currentMinute = static_cast<int>((seconds / 60.f) - (currentHour * 60.f));
      currentSecond = static_cast<int>(seconds) % 60;
    }

    void validate() {
      ENGINE_ASSERT(dayLength > 0.f, "Day length must be positive!");
      ENGINE_ASSERT(startingTime > 0.f, "Starting time must be positive!");
      ENGINE_ASSERT(nightStart > 0.f, "Night start must be positive!");
      ENGINE_ASSERT(dayTransitionStart > 0.f, "Day transition start must be positive!");
      ENGINE_ASSERT(dayStart > 0.f, "Day start must be positive!");
      ENGINE_ASSERT(nightTransitionStart > 0.f, "Night transition start must be positive!");
      ENGINE_ASSERT(acceleration > 0.f, "Acceleration must be positive!");
      ENGINE_ASSERT(fps > 0, "FPS must be positive!");
    }
};
}

template <> void engine::SceneManager::onComponentAdded(Entity& e, CTime& c) {
  c.validate();
  c.setTime(c.startingTime);

  e.update<CTime>(c);
}

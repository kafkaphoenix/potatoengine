#pragma once

#include <entt/entt.hpp>

#include "pch.h"
#include "utils/numericComparator.h"

namespace potatoengine {

struct CTime {
    float seconds{};
    uint32_t currentHour{};
    uint32_t currentMinute{};
    uint32_t currentSecond{};
    float dayLength{0.5};
    float startingTime{7};
    float dayTransitionStart{5};
    float dayStart{7};
    float nightTransitionStart{19};
    float nightStart{21};
    float acceleration{1.f};
    uint32_t fps{60};

    CTime() = default;
    explicit CTime(float s, uint32_t ch, uint32_t cm, uint32_t cs, float st, float dl,
                   float ns, float dts, float ds, float nts, float a, uint32_t f)
      : seconds(s), currentHour(ch), currentMinute(cm), currentSecond(cs),
        startingTime(st), dayLength(dl), nightStart(ns),
        dayTransitionStart(dts), dayStart(ds), nightTransitionStart(nts),
        acceleration(a), fps(f) {}

    void print() const {
      ENGINE_BACKTRACE(
        "\t\tseconds: {0}\n\t\t\t\t\t\tcurrentHour: {1}\n\t\t\t\t\t\tcurrentMinute: "
        "{2}\n\t\t\t\t\t\tcurrentSecond: "
        "{3}\n\t\t\t\t\t\tdayLength: {4}\n\t\t\t\t\t\tstartingTime: "
        "{5}\n\t\t\t\t\t\tdayTransitionStart: "
        "{6}\n\t\t\t\t\t\tdayStart: {7}\n\t\t\t\t\t\tnightTransitionStart: "
        "{8}\n\t\t\t\t\t\tnightStart: "
        "{9}\n\t\t\t\t\t\tacceleration: {10}\n\t\t\t\t\t\tfps: {11}",
        seconds, currentHour, currentMinute, currentSecond, dayLength,
        startingTime, dayTransitionStart, dayStart, nightTransitionStart,
        nightStart, acceleration, fps);
    }

    std::map<std::string, std::string, NumericComparator> getInfo() const {
      std::map<std::string, std::string, NumericComparator> info;
      info["seconds"] = std::to_string(seconds);
      info["currentHour"] = std::to_string(currentHour);
      info["currentMinute"] = std::to_string(currentMinute);
      info["currentSecond"] = std::to_string(currentSecond);
      info["dayLength"] = std::to_string(dayLength);
      info["startingTime"] = std::to_string(startingTime);
      info["dayTransitionStart"] = std::to_string(dayTransitionStart);
      info["dayStart"] = std::to_string(dayStart);
      info["nightTransitionStart"] = std::to_string(nightTransitionStart);
      info["nightStart"] = std::to_string(nightStart);
      info["acceleration"] = std::to_string(acceleration);
      info["fps"] = std::to_string(fps);

      return info;
    }

    void setTime(float time) {
      seconds = time * 3600.f;
      currentHour = static_cast<uint32_t>(seconds / 3600.f);
      currentMinute = static_cast<uint32_t>((seconds / 60.f) - (currentHour * 60.f));
      currentSecond = static_cast<uint32_t>(seconds) % 60;
    }

    void validate() {
      ENGINE_ASSERT(dayLength > 0.f, "Day length must be positive!");
      ENGINE_ASSERT(startingTime > 0.f, "Starting time must be positive!");
      ENGINE_ASSERT(nightStart > 0.f, "Night start must be positive!");
      ENGINE_ASSERT(dayTransitionStart > 0.f,
                    "Day transition start must be positive!");
      ENGINE_ASSERT(dayStart > 0.f, "Day start must be positive!");
      ENGINE_ASSERT(nightTransitionStart > 0.f,
                    "Night transition start must be positive!");
      ENGINE_ASSERT(acceleration > 0.f, "Acceleration must be positive!");
      ENGINE_ASSERT(fps > 0, "FPS must be positive!");
    }
};
}

template <>
inline void engine::SceneManager::onComponentAdded(entt::entity e, CTime& c) {
  c.validate();
  c.setTime(c.startingTime);

  m_registry.replace<CTime>(e, c);
}
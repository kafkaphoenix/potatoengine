#pragma once

#include "potatoengine/scene/entity.h"
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
    explicit CTime(float s, int ch, int cm, int cs, float st, float dl, float ns, float dts, float ds, float nts, float a, int f)
        : seconds(s), currentHour(ch), currentMinute(cm), currentSecond(cs), startingTime(st), dayLength(dl), nightStart(ns), dayTransitionStart(dts), dayStart(ds), nightTransitionStart(nts), acceleration(a), fps(f) {}

    void print() const {
        CORE_TRACE("\t\tseconds: {0}\n\t\t\t\tcurrentHour: {1}\n\t\t\t\tcurrentMinute: {2}\n\t\t\t\tcurrentSecond: {3}\n\t\t\t\tdayLength: {4}\n\t\t\t\tstartingTime: {5}\n\t\t\t\tdayTransitionStart: {6}\n\t\t\t\tdayStart: {7}\n\t\t\t\tnightTransitionStart: {8}\n\t\t\t\tnightStart: {9}\n\t\t\t\tacceleration: {10}\n\t\t\t\tfps: {11}", seconds, currentHour, currentMinute, currentSecond, dayLength, startingTime, dayTransitionStart, dayStart, nightTransitionStart, nightStart, acceleration, fps);
    }

    void setTime(float time) {
        seconds = time * 3600.f;
        currentHour = static_cast<int>(seconds / 3600.f);
        currentMinute = static_cast<int>((seconds / 60.f) - (currentHour * 60.f));
        currentSecond = static_cast<int>(seconds) % 60;
    }
};
}

template <>
void engine::SceneManager::onComponentAdded(Entity e, CTime& c) {
    if (c.dayLength <= 0.f) {
        throw std::runtime_error("Day length must be positive!");
    }

    if (c.startingTime < 0.f) {
        throw std::runtime_error("Starting time must be positive!");
    }

    if (c.nightStart < 0.f) {
        throw std::runtime_error("Night start must be positive!");
    }

    if (c.dayTransitionStart < 0.f) {
        throw std::runtime_error("Day transition start must be positive!");
    }

    if (c.dayStart < 0.f) {
        throw std::runtime_error("Day start must be positive!");
    }

    if (c.nightTransitionStart < 0.f) {
        throw std::runtime_error("Night transition start must be positive!");
    }

    if (c.acceleration < 0.f) {
        throw std::runtime_error("Acceleration must be positive!");
    }

    if (c.fps <= 0) {
        throw std::runtime_error("FPS must be positive!");
    }

    c.setTime(c.startingTime);

    e.update<CTime>(c);
}

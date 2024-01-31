#pragma once

#include "engineAPI.h"

namespace demos {

struct CTimer {
    uint32_t maxTime{20};
    uint32_t left{20};

    CTimer() = default;
    explicit CTimer(uint32_t t, uint32_t l) : maxTime(t), left(l) {}

    void print() const {
      APP_BACKTRACE("\t\tmaxTime: {0}\n\t\t\t\t\t\tleft: {1}", maxTime, left);
    }

    std::map<std::string, std::string, engine::NumericComparator>
    getInfo() const {
      std::map<std::string, std::string, engine::NumericComparator> info;
      info["maxTime"] = std::to_string(maxTime);
      info["left"] = std::to_string(left);

      return info;
    }
};
}
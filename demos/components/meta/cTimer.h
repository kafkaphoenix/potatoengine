#pragma once

#include "engineAPI.h"

namespace demos {

struct CTimer {
    int maxTime{65};
    int left{65};

    CTimer() = default;
    explicit CTimer(int t, int l) : maxTime(t), left(l) {}

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
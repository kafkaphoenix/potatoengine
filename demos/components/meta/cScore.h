#pragma once

#include "engineAPI.h"

namespace demos {

struct CScore {
    int score{};

    CScore() = default;
    explicit CScore(int s) : score(s) {}

    void print() const { APP_BACKTRACE("\t\tscore: {0}", score); }

    std::map<std::string, std::string, engine::NumericComparator>
    getInfo() const {
      std::map<std::string, std::string, engine::NumericComparator> info;
      info["score"] = std::to_string(score);

      return info;
    }
};
}
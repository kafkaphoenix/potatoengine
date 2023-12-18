#pragma once

#include "engineAPI.h"

namespace demos {

struct CExperience {
    int current{};

    CExperience() = default;
    explicit CExperience(int c) : current(c) {}

    void print() const { APP_BACKTRACE("\t\tcurrent: {}", current); }

    std::map<std::string, std::string, engine::NumericComparator> getInfo() const {
      std::map<std::string, std::string, engine::NumericComparator> info;
      info["current"] = std::to_string(current);

      return info;
    }
};
}
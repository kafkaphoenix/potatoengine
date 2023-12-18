#pragma once

#include "engineAPI.h"

namespace demos {

struct CMana {
    int base{};
    int current{};

    CMana() = default;
    explicit CMana(int b) : base(b), current(b) {}

    void print() const {
      APP_BACKTRACE("\t\tbase: {0}\n\t\t\t\tcurrent: {1}", base, current);
    }

    std::map<std::string, std::string, engine::NumericComparator> getInfo() const {
      std::map<std::string, std::string, engine::NumericComparator> info;
      info["base"] = std::to_string(base);
      info["current"] = std::to_string(current);

      return info;
    }
};
}
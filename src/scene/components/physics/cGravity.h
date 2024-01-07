#pragma once

#include "utils/numericComparator.h"

namespace potatoengine {

struct CGravity {
    float acceleration{9.81f};

    CGravity() = default;
    explicit CGravity(float a) : acceleration(a) {}

    void print() const {
      ENGINE_BACKTRACE("\t\tacceleration: {0}", acceleration);
    }

    std::map<std::string, std::string, NumericComparator> getInfo() const {
      std::map<std::string, std::string, NumericComparator> info;
      info["acceleration"] = std::to_string(acceleration);

      return info;
    }
};
}
#pragma once

#include "pch.h"

#include "utils/numericComparator.h"

namespace potatoengine {

struct CDistanceFromCamera {
    int distance{};

    CDistanceFromCamera() = default;
    explicit CDistanceFromCamera(int d) : distance(d) {}

    void print() const { ENGINE_BACKTRACE("\t\tdistance: {}", distance); }

    std::map<std::string, std::string, NumericComparator> getInfo() const {
      std::map<std::string, std::string, NumericComparator> info;
      info["distance"] = std::to_string(distance);

      return info;
    }
};
}
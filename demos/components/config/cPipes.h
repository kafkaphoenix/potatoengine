#pragma once

#include "engineAPI.h"

namespace demos {

struct CPipes {
    int maxPipes{4};
    int pipes{0};

    CPipes() = default;
    explicit CPipes(int mp, int p) : maxPipes(mp), pipes(p) {}

    void print() const {
      APP_BACKTRACE("\t\tmaxPipes: {0}\n\t\t\t\t\t\tpipes: {1}", maxPipes, pipes);
    }

    std::map<std::string, std::string, engine::NumericComparator>
    getInfo() const {
      std::map<std::string, std::string, engine::NumericComparator> info;
      info["maxPipes"] = std::to_string(maxPipes);
      info["pipes"] = std::to_string(pipes);

      return info;
    }
};
}
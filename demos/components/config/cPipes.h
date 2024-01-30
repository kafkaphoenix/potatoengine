#pragma once

#include "engineAPI.h"

namespace demos {

struct CPipes {
    uint32_t maxPipes{4};
    uint32_t pipes{0};

    CPipes() = default;
    explicit CPipes(uint32_t mp, uint32_t p) : maxPipes(mp), pipes(p) {}

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
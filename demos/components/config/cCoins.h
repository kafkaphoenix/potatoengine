#pragma once

#include "engineAPI.h"

namespace demos {

struct CCoins {
    uint32_t maxCoins{4};
    uint32_t coins{};

    CCoins() = default;
    explicit CCoins(uint32_t mp, uint32_t p) : maxCoins(mp), coins(p) {}

    void print() const {
      APP_BACKTRACE("\t\tmaxCoins: {0}\n\t\t\t\t\t\tcoins: {1}", maxCoins,
                    coins);
    }

    std::map<std::string, std::string, engine::NumericComparator>
    getInfo() const {
      std::map<std::string, std::string, engine::NumericComparator> info;
      info["maxCoins"] = std::to_string(maxCoins);
      info["coins"] = std::to_string(coins);

      return info;
    }
};
}
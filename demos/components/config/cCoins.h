#pragma once

#include "engineAPI.h"

namespace demos {

struct CCoins {
    int maxCoins{4};
    int coins{0};

    CCoins() = default;
    explicit CCoins(int mp, int p) : maxCoins(mp), coins(p) {}

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
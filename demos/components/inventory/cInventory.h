#pragma once

#include "components/inventory/cItem.h"
#include "engineAPI.h"

namespace demos {

struct CInventory {
    // TODO define
    std::vector<CItem> items{};

    CInventory() = default;
    explicit CInventory(std::vector<CItem> i) : items(std::move(i)) {}

    void print() const {
      APP_BACKTRACE("\t\titems: No implemented"); // TODO finish
    }

    std::map<std::string, std::string, engine::NumericComparator>
    getInfo() const {
      std::map<std::string, std::string, engine::NumericComparator> info;
      info["items"] = "No implemented"; // TODO finish

      return info;
    }
};
}

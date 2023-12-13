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
    APP_TRACE("\t\titems: No implemented"); // TODO finish
  }
};
}

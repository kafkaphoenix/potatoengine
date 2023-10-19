#pragma once

#include "potatoengine/engineAPI.h"
#include "sandbox/components/inventory/cItem.h"

namespace potatocraft {

struct CInventory {
    // TODO define
    std::vector<CItem> items{};

    CInventory() = default;
    explicit CInventory(std::vector<CItem> i) : items(std::move(i)) {}

    void print() const {
        CORE_INFO("\t\titems: No implemented"); // TODO finish
    }
};
}

#pragma once

#include "potatoengine/engineAPI.h"
#include "demos/components/inventory/cItem.h"

namespace demos{

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

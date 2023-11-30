#pragma once

#include "potatoengine/pch.h"

namespace potatoengine {

struct CUUID {
    uint32_t uuid{};

    void print() const {
        ENGINE_TRACE("\t\tuuid: {}", uuid);
    }
};
}

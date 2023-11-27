#pragma once

#include "potatoengine/pch.h"

namespace potatoengine {

struct CDistanceFromCamera {
    int distance{};

    CDistanceFromCamera() = default;
    explicit CDistanceFromCamera(int d) : distance(d) {}

    void print() const {
        CORE_TRACE("\t\tdistance: {}", distance);
    }
};
}
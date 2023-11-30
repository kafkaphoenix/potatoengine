#pragma once

#include "potatoengine/pch.h"

namespace potatoengine {

struct CDistanceFromCamera {
    int distance{};

    CDistanceFromCamera() = default;
    explicit CDistanceFromCamera(int d) : distance(d) {}

    void print() const {
        ENGINE_TRACE("\t\tdistance: {}", distance);
    }
};
}
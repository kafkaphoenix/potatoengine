#pragma once

#include "pch.h"

namespace potatoengine {

struct CParticle {
    // TODO define
    std::string emitter{};

    CParticle() = default;
    explicit CParticle(std::string&& e) : emitter(std::move(e)) {}

    void print() const { ENGINE_BACKTRACE("\t\temitter: {}", emitter); }
};
}
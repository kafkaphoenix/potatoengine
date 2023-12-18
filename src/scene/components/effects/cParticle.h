#pragma once

#include "pch.h"
#include "utils/numericComparator.h"

namespace potatoengine {

struct CParticle {
    // TODO define
    std::string emitter{};

    CParticle() = default;
    explicit CParticle(std::string&& e) : emitter(std::move(e)) {}

    void print() const { ENGINE_BACKTRACE("\t\temitter: {}", emitter); }

    std::map<std::string, std::string, NumericComparator> getInfo() const {
      std::map<std::string, std::string, NumericComparator> info;
      info["emitter"] = emitter;

      return info;
    }
};
}
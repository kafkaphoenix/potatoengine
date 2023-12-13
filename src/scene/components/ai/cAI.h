#pragma once

#include "pch.h"

namespace potatoengine {

struct CAI {
    // TODO define
    std::string filepath{};

    CAI() = default;
    explicit CAI(std::string&& fp) : filepath(std::move(fp)) {}

    void print() const { ENGINE_TRACE("\t\tfilepath: {}", filepath); }
};
}
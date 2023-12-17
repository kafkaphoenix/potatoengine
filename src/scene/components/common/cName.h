#pragma once

#include "pch.h"

namespace potatoengine {

struct CName {
    std::string name{};

    CName() = default;
    explicit CName(std::string&& n) : name(std::move(n)) {}

    void print() const { ENGINE_BACKTRACE("\t\tname: {}", name); }
};
}
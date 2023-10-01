#pragma once

#include "potatoengine/pch.h"

namespace potatoengine {

struct CAnimation {
    // TODO define
    std::string filepath{};

    CAnimation() = default;
    explicit CAnimation(std::string&& fp) : filepath(std::move(fp)) {}

    void print() const {
        CORE_INFO("\t\tfilepath: {}", filepath);
    }
};
}
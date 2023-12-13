#pragma once

#include "pch.h"

namespace potatoengine {

struct CAnimation {
    // TODO define
    std::string filepath{};

    CAnimation() = default;
    explicit CAnimation(std::string&& fp) : filepath(std::move(fp)) {}

    void print() const {
        ENGINE_TRACE("\t\tfilepath: {}", filepath);
    }
};
}
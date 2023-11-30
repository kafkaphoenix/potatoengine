#pragma once

#include "potatoengine/pch.h"

namespace potatoengine {

struct CTag {
    std::string tag{};

    CTag() = default;
    explicit CTag(std::string&& t) : tag(std::move(t)) {}

    void print() const {
        ENGINE_TRACE("\t\ttag: {}", tag);
    }
};
}
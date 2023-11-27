
#pragma once

#include "potatoengine/pch.h"

namespace potatoengine {

struct CShaderProgram {
    std::string name{};
    bool isVisible{true};

    CShaderProgram() = default;
    explicit CShaderProgram(std::string&& n, bool iv = true)
        : name(std::move(n)), isVisible(iv) {}

    void print() const {
        CORE_TRACE("\t\tname: {0}\n\t\t\t\tisVisible: {1}", name, isVisible);
    }
};
}
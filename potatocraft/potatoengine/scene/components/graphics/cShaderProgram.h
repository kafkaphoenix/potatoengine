
#pragma once

#include "potatoengine/pch.h"

namespace potatoengine {

struct CShaderProgram {
    std::string shaderProgram{};

    CShaderProgram() = default;
    explicit CShaderProgram(std::string&& sp) : shaderProgram(std::move(sp)) {}

    void print() const {
        CORE_INFO("\t\tshaderProgram: {0}", shaderProgram);
    }
};
}
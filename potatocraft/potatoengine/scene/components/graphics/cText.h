#pragma once

#define GLM_FORCE_CTOR_INIT

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#include "potatoengine/pch.h"

namespace potatoengine {

struct CText {
    // font class or freetype?
    std::string text{};
    glm::vec3 color{};

    CText() = default;
    explicit CText(std::string&& t, glm::vec3&& c) : text(std::move(t)), color(std::move(c)) {}

    void print() const {
        CORE_INFO("\t\ttext: {0}\n\t\t\t\tcolor: {1}", text, glm::to_string(color));
    }
};
}
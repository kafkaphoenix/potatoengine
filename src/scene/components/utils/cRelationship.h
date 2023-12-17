#pragma once

#include <entt/entt.hpp>

#include "pch.h"

namespace potatoengine {

struct CRelationship {
    entt::entity parent{entt::null};

    CRelationship() = default;
    explicit CRelationship(entt::entity p) : parent(p) {}

    void print() const { ENGINE_BACKTRACE("\t\tparent UUID: {}", entt::to_integral(parent)); }
};
}
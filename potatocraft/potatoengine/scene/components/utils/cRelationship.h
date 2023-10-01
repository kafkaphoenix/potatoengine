#pragma once

#include <entt/entt.hpp>

#include "potatoengine/pch.h"

namespace potatoengine {

struct CRelationship {
    entt::entity parent{entt::null};

    CRelationship() = default;
    explicit CRelationship(entt::entity p) : parent(p) {}

    void print() const {
        CORE_INFO("\t\tparent UUID: {}", entt::to_integral(parent));
    }
};
}
#pragma once

#include <entt/entt.hpp>

#include "pch.h"
#include "utils/numericComparator.h"

namespace potatoengine {

struct CRelationship {
    entt::entity parent{entt::null};

    CRelationship() = default;
    explicit CRelationship(entt::entity p) : parent(p) {}

    void print() const {
      ENGINE_BACKTRACE("\t\tparent UUID: {}", entt::to_integral(parent));
    }

    std::map<std::string, std::string, NumericComparator> getInfo() const {
      std::map<std::string, std::string, NumericComparator> info;
      info["parent"] = parent not_eq entt::null ? std::to_string(entt::to_integral(parent)) : "undefined parent";

      return info;
    }
};
}
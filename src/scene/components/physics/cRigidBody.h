#pragma once

#include "utils/numericComparator.h"

namespace potatoengine {

struct CRigidBody {
    float mass{};
    float friction{};
    float bounciness{};
    bool isKinematic{};

    CRigidBody() = default;
    explicit CRigidBody(float m, float f, float b, bool k)
      : mass(m), friction(f), bounciness(b), isKinematic(k) {}

    void print() const {
      ENGINE_BACKTRACE("\t\tmass: {0}\n\t\t\t\t\t\tfriction: "
                       "{1}\n\t\t\t\t\t\tbounciness: {2}\n\t\t\t\t\t\tisKinematic: {3}",
                       mass, friction, bounciness, isKinematic);
    }

    std::map<std::string, std::string, NumericComparator> getInfo() const {
      std::map<std::string, std::string, NumericComparator> info;
      info["mass"] = std::to_string(mass);
      info["friction"] = std::to_string(friction);
      info["bounciness"] = std::to_string(bounciness);
      info["isKinematic"] = isKinematic ? "true" : "false";

      return info;
    }
};
}
#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#include "utils/numericComparator.h"

namespace potatoengine {

struct CCollider {
    enum class Type { Box, Capsule, Mesh, Sphere, Rectangle };

    std::string _type{};
    Type type;
    glm::vec3 size{glm::vec3(1.f)};

    CCollider() = default;
    explicit CCollider(Type t, glm::vec3&& s) : type(t), size(std::move(s)) {}

    void print() const {
      ENGINE_BACKTRACE("\t\ttype: {0}\n\t\t\t\tsize: {1}", _type,
                       glm::to_string(size));
    }

    std::map<std::string, std::string, NumericComparator> getInfo() const {
      std::map<std::string, std::string, NumericComparator> info;
      info["type"] = _type;
      info["size"] = glm::to_string(size);

      return info;
    }
};
}

template <>
inline void engine::SceneManager::onComponentAdded(entt::entity e,
                                                   CCollider& c) {
  if (c._type == "box") {
    c.type = CCollider::Type::Box;
  } else if (c._type == "capsule") {
    c.type = CCollider::Type::Capsule;
  } else if (c._type == "mesh") {
    c.type = CCollider::Type::Mesh;
  } else if (c._type == "sphere") {
    c.type = CCollider::Type::Sphere;
  } else if (c._type == "rectangle") {
    c.type = CCollider::Type::Rectangle;
  } else {
    ENGINE_ASSERT(false, "Unknown collider type {}", c._type);
  }

  m_registry.replace<CCollider>(e, c);
}
#pragma once

#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#include "pch.h"
#include "utils/numericComparator.h"

namespace potatoengine {

struct CCollider {
    enum class Type { Box, Capsule, Mesh, Sphere, Rectangle };

    std::string _type;
    Type type;
    glm::vec3 size{glm::vec3(1.f)};

    CCollider() = default;
    explicit CCollider(Type t, glm::vec3&& s) : type(t), size(std::move(s)) {}

    void print() const {
      ENGINE_BACKTRACE("\t\ttype: {0}\n\t\t\t\t\t\tsize: {1}", _type,
                       glm::to_string(size));
    }

    std::map<std::string, std::string, NumericComparator> getInfo() const {
      std::map<std::string, std::string, NumericComparator> info;
      info["type"] = _type;
      info["size"] = glm::to_string(size);

      return info;
    }

    void setColliderType() {
      if (_type == "box") {
        type = Type::Box;
      } else if (_type == "capsule") {
        type = Type::Capsule;
      } else if (_type == "mesh") {
        type = Type::Mesh;
      } else if (_type == "sphere") {
        type = Type::Sphere;
      } else if (_type == "rectangle") {
        type = Type::Rectangle;
      } else {
        ENGINE_ASSERT(false, "Unknown collider type {}", _type);
      }
    }
};
}

template <>
inline void engine::SceneManager::onComponentAdded(entt::entity e,
                                                   CCollider& c) {
  c.setColliderType();

  m_registry.replace<CCollider>(e, c);
}
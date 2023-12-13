#pragma once

#include <glm/glm.hpp>

#include "scene/entity.h"

namespace potatoengine {

struct CCollider {
    enum class Type {
      Box,
      Capsule,
      Mesh,
      Sphere,
    };

    std::string _type{};
    Type type;
    glm::vec3 size{glm::vec3(1.f)};

    CCollider() = default;
    explicit CCollider(Type t, glm::vec3&& s) : type(t), size(std::move(s)) {}

    void print() const { ENGINE_TRACE("\t\ttype: {0}\n\t\t\t\tsize: {1}", _type, size); }
};
}

template <> void engine::SceneManager::onComponentAdded(Entity& e, CCollider& c) {
  if (c._type == "box") {
    c.type = CCollider::Type::Box;
  } else if (c._type == "capsule") {
    c.type = CCollider::Type::Capsule;
  } else if (c._type == "mesh") {
    c.type = CCollider::Type::Mesh;
  } else if (c._type == "sphere") {
    c.type = CCollider::Type::Sphere;
  } else {
    ENGINE_ASSERT(false, "Unknown collider type {}", c._type);
  }

  e.update<CCollider>(c);
}
#pragma once

#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#include "pch.h"
#include "scene/components/graphics/cMesh.h"
#include "utils/mapJsonSerializer.h"
#include "utils/numericComparator.h"
#include "utils/shapeFactory.h"

namespace potatoengine {

struct CShape {
    enum class Type { Triangle, Rectangle, Cube, Circle };

    std::string _type{};
    Type type;
    glm::vec3 size{glm::vec3(1.f)};
    std::vector<CMesh> meshes{};
    bool repeatTexture{};

    CShape() = default;
    explicit CShape(Type t, glm::vec3&& d, std::vector<CMesh>&& m,
                    bool r = false)
      : type(t), size(d), meshes(std::move(m)), repeatTexture(r) {}

    void print() const {
      ENGINE_BACKTRACE("\t\ttype: {0}\n\t\t\t\t\t\tsize: {1}\n\t\t\t\t\t\tmeshes: "
                       "{2}\n\t\t\t\t\t\trepeatTexture: {3}",
                       _type, glm::to_string(size), meshes.size(),
                       repeatTexture);
    }

    std::map<std::string, std::string, NumericComparator> getInfo() const {
      std::map<std::string, std::string, NumericComparator> info;
      info["type"] = _type;
      info["size"] = glm::to_string(size);
      for (int i = 0; i < meshes.size(); ++i) {
        info["mesh " + std::to_string(i)] = getMeshInfo(i);
      }
      info["repeatTexture"] = repeatTexture ? "true" : "false";

      return info;
    }

    std::string getMeshInfo(int index) const {
      return MapToJson(meshes.at(index).getInfo());
    }

    void createMesh() {
      ENGINE_ASSERT(size.x > 0.f and (size.y > 0.f or _type == "triangle"),
                    "Shape witdh and height must be greater than 0");
      ENGINE_ASSERT(size.z > 0.f or _type not_eq "cube",
                    "Cube depth must be greater than 0");
      CMesh mesh;
      if (_type == "triangle") {
        type = CShape::Type::Triangle;
        mesh.vao = ShapeFactory::CreateTriangle(size.x);
        mesh.vertexType = "shape";
      } else if (_type == "rectangle") {
        type = CShape::Type::Rectangle;
        mesh.vao = ShapeFactory::CreateRectangle(size.x, size.y, repeatTexture);
        mesh.vertexType = "shape";
      } else if (_type == "cube") {
        type = CShape::Type::Cube;
        mesh.vao =
          ShapeFactory::CreateCube(size.x, size.y, size.z, repeatTexture);
        mesh.vertexType = "shape";
      } else if (_type == "circle") {
        type = CShape::Type::Circle;
        mesh.vao = ShapeFactory::CreateCircle(size.x, size.y);
        mesh.vertexType = "shape";
      } else {
        ENGINE_ASSERT(false, "Unknown shape type {}", _type);
      }
      meshes.emplace_back(std::move(mesh));
    }
};
}

template <>
inline void engine::SceneManager::onComponentAdded(entt::entity e, CShape& c) {
  c.createMesh();

  m_registry.replace<CShape>(e, c);
}
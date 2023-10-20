#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#include "potatoengine/scene/entity.h"
#include "potatoengine/utils/shapeFactory.h"

namespace potatoengine {

struct CShape {
    enum class Type {
        Triangle,
        Rectangle,
        Cube,
        Circle
    };

    std::string _type{};
    Type type;
    glm::vec3 size{glm::vec3(1.f)};
    std::vector<CMesh> meshes{};
    bool repeatTexture{};

    CShape() = default;
    explicit CShape(Type t, glm::vec3&& d, std::vector<CMesh>&& m, bool r = false) : type(t), size(d), meshes(std::move(m)), repeatTexture(r) {
    }

    void print() const {
        CORE_INFO("\t\ttype: {0}\n\t\t\t\tsize: {1}\n\t\t\t\tmeshes: {2}\n\t\t\t\trepeatTexture: {3}", _type, glm::to_string(size), meshes.size(), repeatTexture);
    }

    void createMesh() {
        if (size.x <= 0.f or (size.y <= 0.f and _type not_eq "triangle")) {
            throw std::runtime_error("Shape's witdh and height must be greater than 0");
        }
        if (size.z <= 0.f and _type == "cube") {
            throw std::runtime_error("Cube's depth must be greater than 0");
        }
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
            mesh.vao = ShapeFactory::CreateCube(size.x, size.y, size.z, repeatTexture);
            mesh.vertexType = "shape";
        } else if (_type == "circle") {
            type = CShape::Type::Circle;
            mesh.vao = ShapeFactory::CreateCircle(size.x, size.y);
            mesh.vertexType = "shape";
        } else {
            throw std::runtime_error("Unknown shape type " + _type);
        }
        meshes.emplace_back(std::move(mesh));
    }
};
}

template <>
void engine::SceneManager::onComponentAdded(Entity e, CShape& c) {
    c.createMesh();
    e.update<CShape>(c);
}
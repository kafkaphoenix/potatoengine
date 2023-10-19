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
    glm::vec3 dimensions{glm::vec3(1.f)};
    std::vector<CMesh> meshes{};
    bool repeatTexture{};

    CShape() = default;
    explicit CShape(Type t, glm::vec3&& d, std::vector<CMesh>&& m) : type(t), dimensions(std::move(d)), meshes(std::move(m)) {}

    void print() const {
        CORE_INFO("\t\ttype: {0}\n\t\t\t\tdimensions: {1}\n\t\t\t\tmeshes: {2}\n\t\t\t\trepeatTexture: {3}", _type, glm::to_string(dimensions), meshes.size(), repeatTexture);
    }
};
}

template <>
void engine::SceneManager::onComponentAdded(Entity e, CShape& c) {
    if (c.dimensions.x <= 0.f or (c.dimensions.y <= 0.f and c._type not_eq "triangle")) {
        throw std::runtime_error("Shape's witdh and height must be greater than 0");
    }
    if (c.dimensions.z <= 0.f and c._type == "cube") {
        throw std::runtime_error("Cube's depth must be greater than 0");
    }
    CMesh mesh;
    if (c._type == "triangle") {
        c.type = CShape::Type::Triangle;
        mesh.vao = ShapeFactory::CreateTriangle(c.dimensions.x);
    } else if (c._type == "rectangle") {
        c.type = CShape::Type::Rectangle;
        mesh.vao = ShapeFactory::CreateRectangle(c.dimensions.x, c.dimensions.y, c.repeatTexture);
    } else if (c._type == "cube") {
        c.type = CShape::Type::Cube;
        mesh.vao = ShapeFactory::CreateCube(c.dimensions.x, c.dimensions.y, c.dimensions.z, c.repeatTexture);
    } else if (c._type == "circle") {
        c.type = CShape::Type::Circle;
        mesh.vao = ShapeFactory::CreateCircle(c.dimensions.x, c.dimensions.y);
    } else {
        throw std::runtime_error("Unknown shape type " + c._type);
    }
    c.meshes.emplace_back(std::move(mesh));
    e.update<CShape>(c);
}
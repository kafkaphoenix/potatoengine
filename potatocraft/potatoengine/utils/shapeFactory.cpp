#include "potatoengine/utils/shapeFactory.h"

#include <numbers>

namespace potatoengine {

std::shared_ptr<VAO> ShapeFactory::CreateShape(const std::vector<ShapeVertex>& vertices, const std::vector<uint32_t>& indices) {
    std::shared_ptr<VAO> vao = VAO::Create();
    vao->attachVertex(VBO::CreateShape(vertices), VAO::VertexType::SHAPE_VERTEX);
    vao->setIndex(IBO::Create(indices));
    return vao;
}

std::shared_ptr<VAO> ShapeFactory::CreateTriangle(float size) {
    std::vector<ShapeVertex> vertices = {
        {{0.f, size, 0.f}, {0.5f, 1.0f}},
        {{-size, -size, 0.f}, {0.f, 0.f}},
        {{size, -size, 0.f}, {1.0f, 0.f}}};

    std::vector<uint32_t> indices = {0, 1, 2};

    return CreateShape(vertices, indices);
}

std::shared_ptr<VAO> ShapeFactory::CreateRectangle(float width, float height, bool repeatTexture) {
    uint32_t overflow = 1;
    if (repeatTexture) {
        if (width not_eq height) {
            throw std::runtime_error("Cannot repeat texture on non-square shape");
        }
        overflow = width;
    }

    std::vector<ShapeVertex> vertices = {
        {{-width, -height, 0.f}, {0.f, 0.f}},
        {{width, -height, 0.f}, {overflow, 0.f}},
        {{width, height, 0.f}, {overflow, overflow}},
        {{-width, height, 0.f}, {0.f, overflow}}};

    std::vector<uint32_t> indices = {
        0, 1, 2,
        2, 3, 0};

    return CreateShape(vertices, indices);
}

std::shared_ptr<VAO> ShapeFactory::CreateCube(float width, float height, float depth, bool repeatTexture) {
    uint32_t overflow = 1;
    if (repeatTexture) {
        if (not(width == height and height == depth)) {
            throw std::runtime_error("Cannot repeat texture on non-square shape");
        }
        overflow = width;
    }

    std::vector<ShapeVertex> vertices = {
        {{-width, -height, -depth}, {0.f, 0.f}},          // A 0
        {{width, -height, -depth}, {overflow, 0.f}},      // B 1
        {{width, height, -depth}, {overflow, overflow}},  // C 2
        {{-width, height, -depth}, {0.f, overflow}},      // D 3
        {{-width, -height, depth}, {0.f, 0.f}},           // E 4
        {{width, -height, depth}, {overflow, 0.f}},       // F 5
        {{width, height, depth}, {overflow, overflow}},   // G 6
        {{-width, height, depth}, {0.f, overflow}},       // H 7

        {{-width, height, -depth}, {0.f, 0.f}},            // D 8
        {{-width, -height, -depth}, {overflow, 0.f}},      // A 9
        {{-width, -height, depth}, {overflow, overflow}},  // E 10
        {{-width, height, depth}, {0.f, overflow}},        // H 11
        {{width, -height, -depth}, {0.f, 0.f}},            // B 12
        {{width, height, -depth}, {overflow, 0.f}},        // C 13
        {{width, height, depth}, {overflow, overflow}},    // G 14
        {{width, -height, depth}, {0.f, overflow}},        // F 15

        {{-width, -height, -depth}, {0.f, 0.f}},          // A 16
        {{width, -height, -depth}, {overflow, 0.f}},      // B 17
        {{width, -height, depth}, {overflow, overflow}},  // F 18
        {{-width, -height, depth}, {0.f, overflow}},      // E 19
        {{width, height, -depth}, {0.f, 0.f}},            // C 20
        {{-width, height, -depth}, {overflow, 0.f}},      // D 21
        {{-width, height, depth}, {overflow, overflow}},  // H 22
        {{width, height, depth}, {0.f, overflow}},        // G 23
    };

    std::vector<uint32_t> indices = {
        // front and back
        0, 3, 2,
        2, 1, 0,
        4, 5, 6,
        6, 7, 4,
        // left and right
        11, 8, 9,
        9, 10, 11,
        12, 13, 14,
        14, 15, 12,
        // bottom and top
        16, 17, 18,
        18, 19, 16,
        20, 21, 22,
        22, 23, 20};

    return CreateShape(vertices, indices);
}

std::shared_ptr<VAO> ShapeFactory::CreateCircle(float radius, uint32_t segments) {
    std::vector<ShapeVertex> vertices;
    std::vector<uint32_t> indices;

    // Center
    vertices.reserve(segments + 1);
    vertices.push_back({{0.f, 0.f, 0.f}, {0.5f, 0.5f}});

    float angleIncrement = 2 * std::numbers::pi / segments;

    // Vertices around the circle
    indices.reserve(segments * 3);
    for (int i = 0; i < segments; ++i) {
        float x = radius * cos(i * angleIncrement);
        float y = radius * sin(i * angleIncrement);
        float u = 0.5f * (1.0f + cos(i * angleIncrement));
        float v = 0.5f * (1.0f + sin(i * angleIncrement));
        vertices.push_back({{x, y, 0.f}, {u, v}});
        indices.push_back(0);
        indices.push_back(i + 1);
        indices.push_back((i + 1) % segments + 1);
    }

    return CreateShape(vertices, indices);
}

}
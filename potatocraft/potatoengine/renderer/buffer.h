#pragma once

#include <glm/glm.hpp>

#include "potatoengine/pch.h"

namespace potatoengine {
#define MAX_BONE_INFLUENCE 4

struct Vertex {
    glm::vec3 position = glm::vec3(0.f);
    glm::vec3 normal = glm::vec3(0.f);
    glm::vec2 textureCoords = glm::vec2(0.f);
    glm::vec3 tangent = glm::vec3(0.f);
    glm::vec3 bitangent = glm::vec3(0.f);
    int boneIDs[MAX_BONE_INFLUENCE];
    float boneWeights[MAX_BONE_INFLUENCE];
    glm::vec4 color = glm::vec4(0.9725f, 0.0f, 0.9725f, 1.0f);
};

class VBO {
   public:
    VBO(const std::vector<Vertex>& vertices, bool immutable);
    ~VBO();

    void reload(const std::vector<Vertex>& vertices) noexcept;

    uint32_t getID() const noexcept { return m_id; }
    const std::vector<Vertex>& getVertices() const noexcept { return m_vertices; }

    static std::unique_ptr<VBO> Create(const std::vector<Vertex>& vertices, bool immutable = true) noexcept;

   private:
    std::vector<Vertex> m_vertices{};
    uint32_t m_id{};
    bool m_immutable{};
};

class IBO {
   public:
    IBO(const std::vector<uint32_t>& indices, bool immutable);
    ~IBO();

    void reload(const std::vector<uint32_t>& indices) noexcept;

    uint32_t getCount() const noexcept { return m_count; }
    uint32_t getID() const noexcept { return m_id; }

    static std::unique_ptr<IBO> Create(const std::vector<uint32_t>& indices, bool immutable = true) noexcept;

   private:
    uint32_t m_id{};
    uint32_t m_count{};
    bool m_immutable{};
};
}
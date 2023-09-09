#pragma once

#define GLM_FORCE_CTOR_INIT
#include <glm/glm.hpp>

#include "potatoengine/pch.h"

namespace potatoengine {
#define MAX_BONE_INFLUENCE 4

struct Vertex { // TODO move
    glm::vec3 position{};
    glm::vec3 normal{};
    glm::vec2 textureCoords{};
    glm::vec3 tangent{};
    glm::vec3 bitangent{};
    int boneIDs[MAX_BONE_INFLUENCE]{};
    float boneWeights[MAX_BONE_INFLUENCE]{};
    glm::vec4 color{glm::vec4(0.9725f, 0.f, 0.9725f, 1.f)};
};

class VBO {
   public:
    VBO(const std::vector<Vertex>& vertices, bool immutable);
    ~VBO();

    void reload(const std::vector<Vertex>& vertices);

    uint32_t getID() const noexcept { return m_id; }

    static std::unique_ptr<VBO> Create(const std::vector<Vertex>& vertices, bool immutable = true);

   private:
    uint32_t m_id{};
    bool m_immutable{};
};

class IBO {
   public:
    IBO(const std::vector<uint32_t>& indices, bool immutable);
    ~IBO();

    void reload(const std::vector<uint32_t>& indices);

    uint32_t getCount() const noexcept { return m_count; }
    uint32_t getID() const noexcept { return m_id; }

    static std::unique_ptr<IBO> Create(const std::vector<uint32_t>& indices, bool immutable = true);

   private:
    uint32_t m_id{};
    uint32_t m_count{};
    bool m_immutable{};
};
}
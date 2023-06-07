#pragma once

#include <glm/glm.hpp>

#include "potatoengine/pch.h"

namespace potatoengine {

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texture;
};

class VBO {
   public:
    VBO(const std::vector<Vertex>& vertices, bool immutable);
    ~VBO();

    void reload(const std::vector<Vertex>& vertices);

    uint32_t getID() const { return m_id; }
    const std::vector<Vertex>& getVertices() const { return m_vertices; }

    static std::unique_ptr<VBO> Create(const std::vector<Vertex>& vertices, bool immutable = true);

   private:
    std::vector<Vertex> m_vertices;
    uint32_t m_id;
    bool m_immutable;
};

class IBO {
   public:
    IBO(const std::vector<uint32_t>& indices, bool immutable);
    ~IBO();

    void reload(const std::vector<uint32_t>& indices);

    uint32_t getCount() const { return m_count; }
    uint32_t getID() const { return m_id; }

    static std::unique_ptr<IBO> Create(const std::vector<uint32_t>& indices, bool immutable = true);

   private:
    uint32_t m_id;
    uint32_t m_count;
    bool m_immutable;
};
}
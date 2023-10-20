#pragma once

#include "potatoengine/renderer/buffer.h"

namespace potatoengine {

class VAO {
   public:
    VAO();
    ~VAO();

    void bind();
    void unbind();

    enum class VertexType { VERTEX, SHAPE_VERTEX, TERRAIN_VERTEX };
    void attachVertex(std::shared_ptr<VBO>&& vbo, VertexType type);
    void attachVertexAttributes();
    void attachShapeVertexAttributes();
    void attachTerrainVertexAttributes();
    void updateVertex(std::unique_ptr<VBO>&& vbo, uint32_t idx, VertexType type);
    void clearVBOs() noexcept;
    void setIndex(std::unique_ptr<IBO>&& ibo);

    const std::vector<std::shared_ptr<VBO>>& getVBOs() const noexcept { return m_vbos; }
    const std::unique_ptr<IBO>& getEBO() const noexcept { return m_ibo; }
    uint32_t getVBOIdx() const noexcept { return m_vboIdx; }
    uint32_t getID() const noexcept { return m_id; }

    static std::shared_ptr<VAO> Create() noexcept;

   private:
    uint32_t m_id;
    uint32_t m_vboIdx = 0;
    std::vector<std::shared_ptr<VBO>> m_vbos;
    std::unique_ptr<IBO> m_ibo;
};

}